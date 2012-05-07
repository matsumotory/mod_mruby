#include "mod_mruby.h"
#include "ap_mrb_request.h"

request_rec *mrb_request_rec_state = NULL;

struct mrb_data_type request_rec_type = {
  "request_rec", 0,
};


int ap_mrb_push_request(request_rec *r)
{
    mrb_request_rec_state = (request_rec *)apr_pcalloc(r->pool, sizeof (*mrb_request_rec_state));
    mrb_request_rec_state = r;
    return OK;
}

request_rec *ap_mrb_get_request()
{
    return mrb_request_rec_state;
}

mrb_value ap_mruby_init_request(mrb_state *mrb, mrb_value str)
{

    str = mrb_class_new_instance(mrb, 0, NULL, class_request);
    mrb_iv_set(mrb
        , str
        , mrb_intern(mrb, "request_rec")
        , mrb_obj_value(Data_Wrap_Struct(mrb
            , mrb->object_class
            , &request_rec_type
            , ap_mrb_get_request())
        )
    );

    return str;
}

mrb_value ap_mrb_get_request_filename(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value val;

    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->filename = apr_pstrdup(r->pool, RSTRING_PTR(val));

    return val;
}

mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value val;

    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->filename = apr_pstrdup(r->pool, RSTRING_PTR(val));

    return val;
}

mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value argc, *argv;
    char *member, *value;
    request_rec *r = ap_mrb_get_request();

    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 2"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    member = RSTRING_PTR(argv[0]);
    value  = RSTRING_PTR(argv[1]);

    if (strcmp(member, "filename") == 0)
        r->filename = apr_pstrdup(r->pool, value);
    else if (strcmp(member, "uri") == 0)
        r->uri = apr_pstrdup(r->pool, value);

    return str;
}
