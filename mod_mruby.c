/*
// -------------------------------------------------------------------
// mod_mruby
//      This is a mruby module for Apache HTTP Server.
//
//      By matsumoto_r (MATSUMOTO, Ryosuke) Sep 2012 in Japan
//          Academic Center for Computing and Media Studies, Kyoto University
//          Graduate School of Informatics, Kyoto University
//          email: matsumoto_r at net.ist.i.kyoto-u.ac.jp
//
// Date     2012/04/21
//
// change log
//  2012/04/21 1.00 matsumoto_r first release
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// How To Compile
// [Use DSO]
// make install
//
// <add to httpd.conf or conf.d/mruby.conf>
// LoadModule mruby_module   modules/mod_mruby.so
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
#ifndef _WIN32
#include "unixd.h"
#endif

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/compile.h>

#include <sys/stat.h>
#include "mod_mruby.h"
#include "ap_mrb_utils.h"
#include "ap_mrb_string.h"
#include "ap_mrb_request.h"
#include "ap_mrb_server.h"
#include "ap_mrb_init.h"

mrb_state *mod_mruby_share_state = NULL;

module AP_MODULE_DECLARE_DATA mruby_module;


#ifdef __MOD_MRUBY_SHARE_CACHE_TABLE__
// shared memory
apr_shm_t *shm;
cache_table_t *shm_table_data_base = NULL;

// grobal mutex 
apr_global_mutex_t *mod_mruby_mutex;
#else
cache_table_t *mod_mruby_cache_table = NULL;
#endif


static int ap_mruby_class_init(mrb_state *mrb);
static int mod_mruby_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s);
static int ap_mruby_class_init(mrb_state *mrb);
static int ap_mruby_run(mrb_state *mrb, request_rec *r, mruby_config_t *conf, const char *mruby_code_file, int module_status);

static void *mod_mruby_create_config(apr_pool_t *p, server_rec *s)
{
    mruby_config_t *conf = 
        (mruby_config_t *) apr_pcalloc(p, sizeof (*conf));

    conf->mod_mruby_handler_code                = NULL;
    conf->mod_mruby_translate_name_first_code   = NULL;
    conf->mod_mruby_translate_name_middle_code  = NULL;
    conf->mod_mruby_translate_name_last_code    = NULL;
    conf->mod_mruby_access_checker_first_code   = NULL;
    conf->mod_mruby_access_checker_middle_code  = NULL;
    conf->mod_mruby_access_checker_last_code    = NULL;
    conf->mod_mruby_check_user_id_first_code    = NULL;
    conf->mod_mruby_check_user_id_middle_code   = NULL;
    conf->mod_mruby_check_user_id_last_code     = NULL;
    conf->mruby_cache_table_size                = 0;

    return conf;
}


static const char *set_mod_mruby_handler(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_handler_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_translate_name_first(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_translate_name_first_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_translate_name_middle(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_translate_name_middle_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_translate_name_last(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_translate_name_last_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_access_checker_first(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_access_checker_first_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_access_checker_middle(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_access_checker_middle_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_access_checker_last(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_access_checker_last_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_check_user_id_first(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_check_user_id_first_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_check_user_id_middle(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_check_user_id_middle_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_check_user_id_last(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf =
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    conf->mod_mruby_check_user_id_last_code = apr_pstrdup(cmd->pool, arg);

    return NULL;
}


static const char *set_mod_mruby_cache_table_size(cmd_parms *cmd, void *mconfig, const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES | NOT_IN_LIMIT);
    mruby_config_t *conf = 
        (mruby_config_t *) ap_get_module_config(cmd->server->module_config, &mruby_module);

    if (err != NULL)
        return err;

    int table_size = strtol(arg, (char **) NULL, 10);
    conf->mruby_cache_table_size = table_size;
    ap_log_perror(APLOG_MARK
        , APLOG_NOTICE
        , 0
        , cmd->pool
        , "%s %s: mod_mruby cache table enabled. table size %d."
        , MODULE_NAME
        , __func__
        , table_size
    );

    return NULL;
}


static int mod_mruby_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s)
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


    ap_log_perror(APLOG_MARK
        , APLOG_NOTICE
        , 0                
        , p
        , "%s %s: main process / thread (pid=%d) initialized."
        , MODULE_NAME
        , __func__
        , getpid()
    );  

    apr_pool_userdata_get(&data, userdata_key, s->process->pool);

    if (!data)
        apr_pool_userdata_set((const void *)1, userdata_key, apr_pool_cleanup_null, s->process->pool);

    ap_log_perror(APLOG_MARK
        , APLOG_NOTICE
        , 0                
        , p
        , "%s %s: %s / %s mechanism enabled."
        , MODULE_NAME
        , __func__
        , MODULE_NAME
        , MODULE_VERSION
    );  
    
    return OK;
}

static int ap_mruby_run(mrb_state *mrb, request_rec *r, mruby_config_t *conf, const char *mruby_code_file, int module_status)
{

    int i, n;
    struct mrb_parser_state* p;
    struct stat st;
    FILE *mrb_file;
    int cache_hit = 0;

    cache_table_t *cache_table_data;
#ifdef __MOD_MRUBY_SHARED_CACHE_TABLE__
    cache_table_data = shm_table_data_base;
#else
    //cache_table_data = (cache_table_t *)apr_pcalloc(r->pool, sizeof(*cache_table_data));
    cache_table_data = mod_mruby_cache_table;
#endif

    if (conf->mruby_cache_table_size > 0) {
        for(i = 0; i < conf->mruby_cache_table_size; i++) {
                ap_log_rerror(APLOG_MARK
                    , APLOG_DEBUG
                    , 0
                    , r
                    , "%s DEBUG %s: %d: %s <=> %s"
                    , MODULE_NAME
                    , __func__
                    , i
                    , mruby_code_file
                    , cache_table_data->cache_code_slot[i].filename
                );
            if (cache_table_data->cache_code_slot[i].filename == NULL)
                continue;

            if (strcmp(cache_table_data->cache_code_slot[i].filename, mruby_code_file) == 0) {
                if (stat(mruby_code_file, &st) == -1) {
                    ap_log_rerror(APLOG_MARK
                        , APLOG_ERR
                        , 0
                        , r
                        , "%s ERROR %s: cache check phase. stat failed: %s"
                        , MODULE_NAME
                        , __func__
                        , mruby_code_file
                    );
                    return DECLINED;
                }
                if (cache_table_data->cache_code_slot[i].stat_mtime != (int)st.st_mtime) {
                    ap_log_rerror(APLOG_MARK
                        , APLOG_DEBUG
                        , 0
                        , r
                        , "%s DEBUG %s: stat changed. cache deleted: %s"
                        , MODULE_NAME
                        , __func__
                        , mruby_code_file
                    );
                    mod_mruby_cache_table->cache_code_slot[i].filename   = NULL;
                    mod_mruby_cache_table->cache_code_slot[i].mrb        = NULL;
                    mod_mruby_cache_table->cache_code_slot[i].cache_id   = -1;
                    mod_mruby_cache_table->cache_code_slot[i].ireq_id    = -1;
                    mod_mruby_cache_table->cache_code_slot[i].stat_mtime = -1;

                    continue;
                }
                n = cache_table_data->cache_code_slot[i].ireq_id;
                mrb = cache_table_data->cache_code_slot[i].mrb;
                cache_hit = 1;
                ap_log_rerror(APLOG_MARK
                    , APLOG_DEBUG
                    , 0
                    , r
                    , "%s DEBUG %s: cache hits! on pid %d: %s"
                    , MODULE_NAME
                    , __func__
                    , getpid()
                    , mruby_code_file
                );
                break;
            }
        }
    }

    if (!cache_hit) {
        if ((mrb_file = fopen(mruby_code_file, "r")) == NULL) {
            ap_log_error(APLOG_MARK
                , APLOG_ERR
                , 0
                , NULL
                , "%s ERROR %s: mrb file oepn failed: %s"
                , MODULE_NAME
                , __func__
                , mruby_code_file
            );
        }

       ap_log_rerror(APLOG_MARK
           , APLOG_DEBUG
           , 0
           , r
           , "%s DEBUG %s: cache nothing on pid %d, compile code: %s"
           , MODULE_NAME
           , __func__
           , getpid()
           , mruby_code_file
       );

        //mod_mruby_share_state = mrb_open();
        //ap_mruby_class_init(mod_mruby_share_state);
        //mrb = mod_mruby_share_state;

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
                , mruby_code_file
            );
            return OK;
        }
#endif
        if (conf->mruby_cache_table_size > 0) {
            if (stat(mruby_code_file, &st) == -1) {
                ap_log_rerror(APLOG_MARK
                    , APLOG_ERR
                    , 0 
                    , r 
                    , "%s ERROR %s: cache update phase. stat failed: %s"
                    , MODULE_NAME
                    , __func__
                    , mruby_code_file
                );  
                return DECLINED;
            }   

            for(i = 0; i <= conf->mruby_cache_table_size; i++) {
                if (cache_table_data->cache_code_slot[i].cache_id == -1) {
                    cache_table_data->cache_code_slot[i].filename = (char *)apr_pcalloc(r->pool, sizeof(mruby_code_file));
                    cache_table_data->cache_code_slot[i].filename = apr_pstrdup(r->pool, mruby_code_file);
                    cache_table_data->cache_code_slot[i].mrb = (mrb_state *)apr_pcalloc(r->pool, sizeof(*mrb)); 
                    cache_table_data->cache_code_slot[i].mrb = mrb; 
                    cache_table_data->cache_code_slot[i].ireq_id = n;
                    cache_table_data->cache_code_slot[i].cache_id = i; 
                    cache_table_data->cache_code_slot[i].stat_mtime = (int)st.st_mtime; 

                    ap_log_rerror(APLOG_MARK
                        , APLOG_DEBUG
                        , 0
                        , r
                        , "%s DEBUG %s: cache created: %s"
                        , MODULE_NAME
                        , __func__
                        , mruby_code_file
                    );

                    break;
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
                , mruby_code_file
            );
            return OK;
        }
#endif
        mrb_pool_close(p->pool);
    }

    ap_log_rerror(APLOG_MARK
        , APLOG_DEBUG
        , 0
        , r
        , "%s DEBUG %s: run mruby code: %s"
        , MODULE_NAME
        , __func__
        , mruby_code_file
    );

    ap_mrb_set_status_code(OK);
    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_nil_value());

    ap_log_rerror(APLOG_MARK
        , APLOG_DEBUG
        , 0
        , r
        , "%s DEBUG %s: return mruby code(%d): %s"
        , MODULE_NAME
        , __func__
        , ap_mrb_get_status_code()
        , mruby_code_file
    );

    return ap_mrb_get_status_code();
}


static void mod_mruby_child_init(apr_pool_t *pool, server_rec *server)
{

    int i;
    mruby_config_t *conf = ap_get_module_config(server->module_config, &mruby_module);

    mod_mruby_cache_table =
        (cache_table_t *)apr_pcalloc(pool , sizeof(*mod_mruby_cache_table));
    mod_mruby_cache_table->cache_code_slot =
        (cache_code_t *)apr_pcalloc(pool, sizeof(mod_mruby_cache_table->cache_code_slot) * conf->mruby_cache_table_size);

    mod_mruby_share_state = mrb_open();
    ap_mruby_class_init(mod_mruby_share_state);

    if (conf->mruby_cache_table_size > 0) {
        for (i = 0; i < conf->mruby_cache_table_size; i++) {
            mod_mruby_cache_table->cache_code_slot[i].filename   = NULL;
            mod_mruby_cache_table->cache_code_slot[i].mrb        = NULL;
            mod_mruby_cache_table->cache_code_slot[i].cache_id   = -1;
            mod_mruby_cache_table->cache_code_slot[i].ireq_id    = -1;
            mod_mruby_cache_table->cache_code_slot[i].stat_mtime = -1;
        }
        ap_log_perror(APLOG_MARK
            , APLOG_NOTICE
            , 0
            , pool
            , "%s NOTICE %s: cache initialized."
            , MODULE_NAME
            , __func__
        );
    }

    ap_log_perror(APLOG_MARK
        , APLOG_NOTICE
        , 0
        , pool
        , "%s %s: child process (pid=%d) initialized."
        , MODULE_NAME
        , __func__
        , getpid()
    );
}


static int mod_mruby_handler(request_rec *r)
{

    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);

    if (strcmp(r->handler, "mruby-script") == 0)
        conf->mod_mruby_handler_code = apr_pstrdup(r->pool, r->filename);
    else
        return DECLINED;

    ap_mrb_push_request(r);
    
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_handler_code, OK);
}


static int mod_mruby_translate_name_first(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_translate_name_first_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_translate_name_first_code, OK);
}


static int mod_mruby_translate_name_middle(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_translate_name_middle_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_translate_name_middle_code, OK);
}


static int mod_mruby_translate_name_last(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_translate_name_last_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_translate_name_last_code, OK);
}


static int mod_mruby_access_checker_first(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_access_checker_first_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_access_checker_first_code, OK);
}


static int mod_mruby_access_checker_middle(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_access_checker_middle_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_access_checker_middle_code, OK);
}


static int mod_mruby_access_checker_last(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_access_checker_last_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_access_checker_last_code, OK);
}


static int mod_mruby_check_user_id_first(request_rec *r)
{
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_check_user_id_first_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_check_user_id_first_code, OK);
}


static int mod_mruby_check_user_id_middle(request_rec *r)
{   
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_check_user_id_middle_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_check_user_id_middle_code, OK);
}


static int mod_mruby_check_user_id_last(request_rec *r)
{   
    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);
    if (conf->mod_mruby_check_user_id_last_code == NULL)
        return DECLINED;
    ap_mrb_push_request(r);
    return ap_mruby_run(mod_mruby_share_state, r, conf, conf->mod_mruby_check_user_id_last_code, OK);
}


static void register_hooks(apr_pool_t *p)
{
    ap_hook_post_config(mod_mruby_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(mod_mruby_child_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(mod_mruby_handler, NULL, NULL, APR_HOOK_REALLY_FIRST);
    ap_hook_translate_name(mod_mruby_translate_name_first, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_translate_name(mod_mruby_translate_name_middle, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_translate_name(mod_mruby_translate_name_last, NULL, NULL, APR_HOOK_LAST);
    ap_hook_access_checker(mod_mruby_access_checker_first, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_access_checker(mod_mruby_access_checker_middle, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_access_checker(mod_mruby_access_checker_last, NULL, NULL, APR_HOOK_LAST);
    ap_hook_check_user_id(mod_mruby_check_user_id_first, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_check_user_id(mod_mruby_check_user_id_middle, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_user_id(mod_mruby_check_user_id_last, NULL, NULL, APR_HOOK_LAST);
}


static const command_rec mod_mruby_cmds[] = {

    AP_INIT_TAKE1("mrubyHandler", set_mod_mruby_handler, NULL, RSRC_CONF | ACCESS_CONF, "hook for handler phase."),
    AP_INIT_TAKE1("mrubyTranslateNameFirst", set_mod_mruby_translate_name_first, NULL, RSRC_CONF | ACCESS_CONF, "hook for translate_name first phase."),
    AP_INIT_TAKE1("mrubyTranslateNameMiddle", set_mod_mruby_translate_name_middle, NULL, RSRC_CONF | ACCESS_CONF, "hook for translate_name middle phase."),
    AP_INIT_TAKE1("mrubyTranslateNameLast", set_mod_mruby_translate_name_last, NULL, RSRC_CONF | ACCESS_CONF, "hook for translate_name last phase."),
    AP_INIT_TAKE1("mrubyAccessCheckerFirst", set_mod_mruby_access_checker_first, NULL, RSRC_CONF | ACCESS_CONF, "hook for access_checker first phase."),
    AP_INIT_TAKE1("mrubyAccessCheckerMiddle", set_mod_mruby_access_checker_middle, NULL, RSRC_CONF | ACCESS_CONF, "hook for access_checker middle phase."),
    AP_INIT_TAKE1("mrubyAccessCheckerLast", set_mod_mruby_access_checker_last, NULL, RSRC_CONF | ACCESS_CONF, "hook for access_checker last phase."),
    AP_INIT_TAKE1("mrubyCheckUserIdFirst", set_mod_mruby_check_user_id_first, NULL, RSRC_CONF | ACCESS_CONF, "hook for check_user_id first phase."),
    AP_INIT_TAKE1("mrubyCheckUserIdMiddle", set_mod_mruby_check_user_id_middle, NULL, RSRC_CONF | ACCESS_CONF, "hook for check_user_id middle phase."),
    AP_INIT_TAKE1("mrubyCheckUserIdLast", set_mod_mruby_check_user_id_last, NULL, RSRC_CONF | ACCESS_CONF, "hook for check_user_id last phase."),
    AP_INIT_TAKE1("mrubyCacheSize", set_mod_mruby_cache_table_size, NULL, RSRC_CONF | ACCESS_CONF, "set mruby cache table size."),
    {NULL}
};


module AP_MODULE_DECLARE_DATA mruby_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                      /* dir config creater */
    NULL,                      /* dir merger */
    mod_mruby_create_config,   /* server config */
    NULL,                      /* merge server config */
    mod_mruby_cmds,                /* command apr_table_t */
    register_hooks             /* register hooks */
};
