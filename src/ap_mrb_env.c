/*
// ap_mrb_env.c - to provide env methods
//
// See Copyright Notice in mod_mruby.h
*/

#include "ap_mrb_env.h"
#include "mruby/hash.h"
#include "util_script.h"

mrb_value ap_mrb_init_env(mrb_state *mrb, mrb_value self)
{
    request_rec *r = ap_mrb_get_request();
    ap_add_common_vars(r);
    ap_add_cgi_vars(r);
    ap_mrb_push_request(r);

    return self;
}


mrb_value ap_mrb_set_env(mrb_state *mrb, mrb_value str)
{
    mrb_value key, val;

    mrb_get_args(mrb, "oo", &key, &val);
    request_rec *r = ap_mrb_get_request();

    apr_table_t *e = r->subprocess_env;
    //putenv(apr_psprintf(r->pool, "%s=%s", RSTRING_PTR(key), RSTRING_PTR(val)));
    apr_table_setn(e, RSTRING_PTR(key), RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_get_env(mrb_state *mrb, mrb_value str)
{
    mrb_value key;

    mrb_get_args(mrb, "o", &key);
    request_rec *r = ap_mrb_get_request();
    
    apr_table_t *e = r->subprocess_env;
    //const char *val = getenv((const char*)RSTRING_PTR(key));
    const char *val = apr_table_get(e, RSTRING_PTR(key));
    if (val == NULL)
        return mrb_nil_value();
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_env_hash(mrb_state *mrb, mrb_value str)
{
    int i;
    mrb_value hash = mrb_hash_new(mrb);
    request_rec *r = ap_mrb_get_request();
    apr_table_t *e = r->subprocess_env;
    const apr_array_header_t *arr = apr_table_elts(e);
    apr_table_entry_t *elts = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        mrb_hash_set(mrb
            , hash
            , mrb_str_new(mrb, elts[i].key, strlen(elts[i].key))
            , mrb_str_new(mrb, elts[i].val, strlen(elts[i].val))
        );
    }
    return hash;
}

void ap_mruby_env_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_env;

    class_env = mrb_define_class_under(mrb, class_core, "Env", mrb->object_class);
    mrb_define_method(mrb, class_env, "initialize", ap_mrb_init_env, ARGS_NONE());
    mrb_define_method(mrb, class_env, "[]=", ap_mrb_set_env, ARGS_ANY());
    mrb_define_method(mrb, class_env, "[]", ap_mrb_get_env, ARGS_ANY());
    mrb_define_method(mrb, class_env, "env_hash", ap_mrb_get_env_hash, ARGS_NONE());
}
