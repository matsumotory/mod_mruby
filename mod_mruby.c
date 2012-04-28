/*
// -------------------------------------------------------------------
// mod_mruby
//      This is a mruby module for Apache HTTP Server.
//
//      By matsumoto_r (MATSUMOTO, Ryosuke) Sep 2012 in Japan
//          Academic Center for Computing and Media Studies, Kyoto University
//          email: matsumoto_r at net.ist.i.kyoto-u.ac.jp
//
// Date     2012/04/21
// Version  0.01
//
// change log
//  2012/04/21 1.00 matsumoto_r first release
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// How To Compile
// [Use DSO]
// apxs -i -c -l cap mod_mruby.c
//
// <add to httpd.conf or conf.d/mruby.conf>
// LoadModule mruby_module   modules/mod_mruby.so
//
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// How To Use
//
//  * Set mruby for handler phase
//      mrubyHandler /path/to/file.mrb
//
// -------------------------------------------------------------------
*/

#include "apr_strings.h"
#include "ap_config.h"
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"

#include "apr_global_mutex.h"
#include "unixd.h"

#include <mruby.h>
#include <mruby/proc.h>
#include <compile.h>

#define MODULE_NAME        "mod_mruby"
#define MODULE_VERSION     "0.01"
#define UNSET              -1
#define SET                1
#define ON                 1
#define OFF                0

#include "ap_mrb_utils.h"
#include "ap_mrb_string.h"

module AP_MODULE_DECLARE_DATA mruby_module;

typedef struct {

    const char *mruby_code_file;
    int mruby_cache_table_size;

} mruby_config_t;

typedef struct cache_code_str {

    int cache_id;
    int ireq_id;
    char *filename;
    mrb_state *mrb;

} cache_code_t;

typedef struct cache_shm_table {

    cache_code_t *cache_code_slot;

} cache_table_t;

#ifdef __MOD_MRUBY_SHARE_CACHE_TABLE__
// shared memory
apr_shm_t *shm;
cache_table_t *shm_table_data_base = NULL;

// grobal mutex 
apr_global_mutex_t *mod_mruby_mutex;
#else
cache_table_t *mod_mruby_cache_table = NULL;
#endif


static void *create_config(apr_pool_t *p, server_rec *s)
{
    mruby_config_t *conf = 
        (mruby_config_t *) apr_pcalloc(p, sizeof (*conf));

    conf->mruby_code_file = NULL;
    conf->mruby_cache_table_size = 0;

    return conf;
}


static const char *set_mruby_handler(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mruby_code_file = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mruby_cache_table_size(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    signed long int table_size = strtol(arg, (char **) NULL, 10);
    conf->mruby_cache_table_size = table_size;

    mod_mruby_cache_table = 
        (cache_table_t *) apr_pcalloc(cmd->pool , sizeof(*mod_mruby_cache_table));
    mod_mruby_cache_table->cache_code_slot = 
        (cache_code_t *)apr_pcalloc(cmd->pool, sizeof(mod_mruby_cache_table->cache_code_slot) * table_size);

    int i;
    for (i = 0; i < table_size; i++) {
        mod_mruby_cache_table->cache_code_slot[i].filename = NULL;
        mod_mruby_cache_table->cache_code_slot[i].mrb      = NULL;
        mod_mruby_cache_table->cache_code_slot[i].cache_id = -1;
        mod_mruby_cache_table->cache_code_slot[i].ireq_id  = -1;
    }

    return NULL;
}


static int mruby_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s)
{
    void *data;
    const char *userdata_key = "mruby_init";

#ifdef __MOD_MRUBY_SHARE_CACHE_TABLE__
    cache_table_t *cache_table_init = NULL;

    apr_status_t status;
    apr_size_t retsize;
    apr_size_t shm_size;
    
    
    shm_size = (apr_size_t) (sizeof(shm_table_data) + sizeof(shm_table_data->cache_code_slot) * MOD_MRUBY_CACHE_TABLE_SIZE);
    
    //Create global mutex
    status = apr_global_mutex_create(&mod_mruby_mutex, NULL, APR_LOCK_DEFAULT, p);
    if(status != APR_SUCCESS){
        ap_log_error(APLOG_MARK
            , APLOG_ERR        
            , 0                
            , NULL             
            , "%s ERROR %s: Error creating global mutex."
            , MODULE_NAME
            , __func__
        );

        return status;
    }   
#ifdef AP_NEED_SET_MUTEX_PERMS
    status = unixd_set_global_mutex_perms(mod_mruby_mutex);
    if(status != APR_SUCCESS){
        ap_log_error(APLOG_MARK
            , APLOG_ERR        
            , 0                
            , NULL             
            , "%s ERROR %s: Error xrent could not set permissions on global mutex."
            , MODULE_NAME
            , __func__
        );
        return status;
    }   
#endif

    if(apr_global_mutex_child_init(&mod_mruby_mutex, NULL, p)) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR        
            , 0                
            , NULL             
            , "%s ERROR %s: global mutex attached."
            , MODULE_NAME
            , __func__
        );
    }
        
    /* If there was a memory block already assigned.. destroy it */
    if (shm) {
        status = apr_shm_destroy(shm);
        if (status != APR_SUCCESS) {
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0                
                , NULL
                , "%s ERROR %s: Couldn't destroy old memory block."
                , MODULE_NAME
                , __func__
            );  
            return status;
        } else {
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0                
                , NULL
                , "%s ERROR %s: Old Shared memory block, destroyed."
                , MODULE_NAME
                , __func__
            );  
        }   
    }   
    
    /* Create shared memory block */
    status = apr_shm_create(&shm, shm_size, NULL, p);
    if (status != APR_SUCCESS) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0                
            , NULL
            , "%s ERROR %s: Error creating shm block."
            , MODULE_NAME
            , __func__
        );  
        return status;
    }

    /* Check size of shared memory block */
    retsize = apr_shm_size_get(shm);
    if (retsize != shm_size) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0                
            , NULL
            , "%s ERROR %s: Error allocating shared memory block."
            , MODULE_NAME
            , __func__
        );  
        return status;
    }
    /* Init shm block */
    shm_table_data_base = apr_shm_baseaddr_get(shm);
    if (shm_table_data_base == NULL) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0                
            , NULL
            , "%s ERROR %s: Error creating status block."
            , MODULE_NAME
            , __func__
        );  
        return status;
    }
    memset(shm_table_data_base, 0, retsize);

    ap_log_error(APLOG_MARK
        , APLOG_ERR
        , 0                
        , NULL
        , "%s INFO %s: Memory Allocated %d bytes (each cache takes %d bytes) MRUBY_CHACHE_TABLE_SIZE:%d"
        , MODULE_NAME
        , __func__
        , (int) retsize
        , (int) (sizeof(shm_table_data) + sizeof(shm_table_data->cache_code_slot))
        , MOD_MRUBY_CACHE_TABLE_SIZE
    );  

    if (retsize < sizeof(shm_table_data)) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0                
            , NULL
            , "%s ERROR %s: Not enough memory allocated!! Giving up."
            , MODULE_NAME
            , __func__
        );  
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    cache_table_init = shm_table_data_base;

    for (i = 0; i < MOD_MRUBY_CACHE_TABLE_SIZE; i++) {
        cache_table_init->cache_code_slot[i].filename = NULL;
        cache_table_init->cache_code_slot[i].mrb      = NULL;
        cache_table_init->cache_code_slot[i].cache_id = -1;
        cache_table_init->cache_code_slot[i].ireq_id  = -1;
    }
#endif

    ap_log_error(APLOG_MARK
        , APLOG_ERR
        , 0                
        , NULL
        , "%s INFO %s: Initialized."
        , MODULE_NAME
        , __func__
    );  

    apr_pool_userdata_get(&data, userdata_key, s->process->pool);

    if (!data)
        apr_pool_userdata_set((const void *)1, userdata_key, apr_pool_cleanup_null, s->process->pool);

    ap_log_error(APLOG_MARK
        , APLOG_ERR
        , 0                
        , NULL
        , "%s INFO %s: %s mechanism enabled."
        , MODULE_NAME
        , __func__
        , MODULE_NAME
    );  
    
    return OK;
}

static int ap_mruby_class_init(mrb_state *mrb)
{

    struct RClass *class;

    class = mrb_define_module(mrb, "Apache");
    mrb_define_class_method(mrb, class, "sleep", ap_mrb_sleep, ARGS_ANY());
    mrb_define_class_method(mrb, class, "rputs", ap_mrb_rputs, ARGS_ANY());
    mrb_define_class_method(mrb, class, "errlogger", ap_mrb_errlogger, ARGS_ANY());
    mrb_define_class_method(mrb, class, "syslogger", ap_mrb_syslogger, ARGS_ANY());

    return OK;

}

static int ap_mruby_run(mrb_state *mrb, request_rec *r,  mruby_config_t *conf)
{

    int i, n;
    struct mrb_parser_state* p;
    FILE *mrb_file;
    int cache_hit = 0;

    cache_table_t *cache_table_data;
#ifdef __MOD_MRUBY_SHARED_CACHE_TABLE__
    cache_table_data = shm_table_data_base;
#else
    cache_table_data = mod_mruby_cache_table;
#endif

    if (conf->mruby_cache_table_size > 0) {
        for(i = 0; i < conf->mruby_cache_table_size; i++) {
            if (strcmp(cache_table_data->cache_code_slot[i].filename, conf->mruby_code_file) == 0) {
                n = cache_table_data->cache_code_slot[i].ireq_id;
                mrb = cache_table_data->cache_code_slot[i].mrb;
                cache_hit = 1;
                ap_log_error(APLOG_MARK
                    , APLOG_ERR
                    , 0
                    , NULL
                    , "%s DEBUG %s: cache hits!: %s"
                    , MODULE_NAME
                    , __func__
                    , conf->mruby_code_file
                );
                break;
            }
        }
    }

    if (cache_hit == 0) {
        if ((mrb_file = fopen(conf->mruby_code_file, "r")) == NULL) {
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0
                , NULL
                , "%s ERROR %s: mrb file oepn failed: %s"
                , MODULE_NAME
                , __func__
                , conf->mruby_code_file
            );
        }

       ap_log_error(APLOG_MARK
           , APLOG_ERR
           , 0
           , NULL
           , "%s DEBUG %s: cache nothing, compile code.: %s"
           , MODULE_NAME
           , __func__
           , conf->mruby_code_file
       );

        p = mrb_parse_file(mrb, mrb_file);
        n = mrb_generate_code(mrb, p->tree);

#ifdef __MOD_MRUBY_SHARED_CACHE_TABLE__
    // mod_mruby_mutex lock
        if (apr_global_mutex_lock(mod_mruby_mutex) != APR_SUCCESS) {
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0
                , NULL
                , "%s ERROR %s: mod_mruby_mutex lock failed: %s"
                , MODULE_NAME
                , __func__
                , conf->mruby_code_file
            );
            return OK;
        }
#endif
        if (conf->mruby_cache_table_size > 0) {
            for(i = 0; i <= conf->mruby_cache_table_size; i++) {
                if (cache_table_data->cache_code_slot[i].cache_id == -1) {
                    cache_table_data->cache_code_slot[i].filename = apr_pstrdup(r->pool, conf->mruby_code_file);
                    cache_table_data->cache_code_slot[i].mrb = mrb; 
                    cache_table_data->cache_code_slot[i].ireq_id = n;
                    cache_table_data->cache_code_slot[i].cache_id = i; 
                }
            }
        }
#ifdef __MOD_MRUBY_SHARED_CACHE_TABLE__
        // mod_mruby_mutex unlock
        if (apr_global_mutex_unlock(mod_mruby_mutex) != APR_SUCCESS){
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0
                , NULL
                , "%s ERROR %s: mod_mruby_mutex unlock failed: %s"
                , MODULE_NAME
                , __func__
                , conf->mruby_code_file
            );
            return OK;
        }
#endif
        mrb_pool_close(p->pool);
    }

    ap_log_error(APLOG_MARK
        , APLOG_ERR
        , 0
        , NULL
        , "%s DEBUG %s: run mruby code.: %s"
        , MODULE_NAME
        , __func__
        , conf->mruby_code_file
    );

    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_nil_value());

    return OK;
}

static int mruby_handler(request_rec *r)
{

    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);

    if (strcmp(r->handler, "mruby-script") == 0)
        conf->mruby_code_file = apr_pstrdup(r->pool, r->filename);
    else
        return DECLINED;

    ap_mrb_push_request(r);
    mrb_state *mrb = mrb_open();
    ap_mruby_class_init(mrb);
    
    return ap_mruby_run(mrb, r, conf);
}


static const command_rec mruby_cmds[] = {

    AP_INIT_TAKE1("mrubyHandler", set_mruby_handler, NULL, RSRC_CONF | ACCESS_CONF, "hook for handler phase."),
    AP_INIT_TAKE1("mrubyCacheSize", set_mruby_cache_table_size, NULL, RSRC_CONF | ACCESS_CONF, "set mruby cache table size."),
    {NULL}
};


static void register_hooks(apr_pool_t *p)
{
    ap_hook_post_config(mruby_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(mruby_handler, NULL, NULL, APR_HOOK_REALLY_FIRST);
}


module AP_MODULE_DECLARE_DATA mruby_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                      /* dir config creater */
    NULL,                      /* dir merger */
    create_config,             /* server config */
    NULL,                      /* merge server config */
    mruby_cmds,                /* command apr_table_t */
    register_hooks             /* register hooks */
};
