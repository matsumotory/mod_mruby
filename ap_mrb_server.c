#include "mod_mruby.h"
#include "ap_mrb_server.h"
//#include "json.h"

//static struct mrb_data_type server_rec_type = {
//    "server_rec", 0,
//};

// char
mrb_value ap_mrb_set_server_error_fname(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->server->error_fname = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_get_server_error_fname(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->server->error_fname));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_server_document_root(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_document_root(r));
    return mrb_str_new(mrb, val, strlen(val));
}

// int

mrb_value ap_mrb_set_server_loglevel(mrb_state *mrb, mrb_value str)
{
    mrb_int val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "i", &val);
#ifdef __APACHE24__
    r->server->log.level = (int)val;
#else
    r->server->loglevel = (int)val;
#endif
    return str;
}

mrb_value ap_mrb_get_server_loglevel(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
#ifdef __APACHE24__
    return mrb_fixnum_value(r->server->log.level);
#else
    return mrb_fixnum_value(r->server->loglevel);
#endif
}
