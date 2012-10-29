#include "ap_mrb_env.h"

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
