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

#include <mruby.h>
#include <mruby/proc.h>
#include <compile.h>

#include "ap_mrb_utils.h"
#include "ap_mrb_string.h"

#define MODULE_NAME        "mod_mruby"
#define MODULE_VERSION     "0.01"
#define UNSET              -1
#define SET                1
#define ON                 1
#define OFF                0


typedef struct {

    const char *mruby_code_file;

} mruby_config_t;

module AP_MODULE_DECLARE_DATA mruby_module;

static void *create_config(apr_pool_t *p, server_rec *s)
{
    mruby_config_t *conf = 
        (mruby_config_t *) apr_pcalloc(p, sizeof (*conf));

    conf->mruby_code_file = NULL;

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


static int mruby_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s)
{
    void *data;
    const char *userdata_key = "mruby_init";

    apr_pool_userdata_get(&data, userdata_key, s->process->pool);

    if (!data)
        apr_pool_userdata_set((const void *)1, userdata_key, apr_pool_cleanup_null, s->process->pool);
    else                                              
        ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL, MODULE_NAME "/" MODULE_VERSION " mechanism enabled");
    
    return OK;
}

static int ap_mruby_class_init(mrb_state *mrb)
{

    struct RClass *class;

    class = mrb_define_module(mrb, "Apache");
    mrb_define_class_method(mrb, class, "sleep", ap_mrb_sleep, ARGS_ANY());
    mrb_define_class_method(mrb, class, "rputs", ap_mrb_rputs, ARGS_ANY());
    mrb_define_class_method(mrb, class, "errlogger", ap_mrb_err_logger, ARGS_ANY());

    return OK;

}

static int ap_mruby_run(mrb_state *mrb, const char *code_file)
{

    struct mrb_parser_state* p;
    int n;
    FILE *mrb_file;

    if ((mrb_file = fopen(code_file, "r")) == NULL) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0
            , NULL
            , "%s ERROR %s: mrb file oepn failed: %s"
            , MODULE_NAME
            , __func__
            , code_file
        );

    }
    p = mrb_parse_file(mrb, mrb_file);
    n = mrb_generate_code(mrb, p->tree);
    mrb_pool_close(p->pool);
    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_nil_value());

    return OK;
}

static int mruby_handler(request_rec *r)
{

    mruby_config_t *conf = ap_get_module_config(r->server->module_config, &mruby_module);

    ap_mrb_push_request(r);

    mrb_state *mrb = mrb_open();
    ap_mruby_class_init(mrb);
    
    return ap_mruby_run(mrb, conf->mruby_code_file);
}


static const command_rec mruby_cmds[] = {

    AP_INIT_TAKE1("mrubyHandler", set_mruby_handler, NULL, RSRC_CONF | ACCESS_CONF, "hook for handler phase"),
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
