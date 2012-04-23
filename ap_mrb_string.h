#include "mruby/string.h"

request_rec *mrb_request_rec_state = NULL;

static int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();
mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str);
//mrb_value ap_mrb_rputs_test(mrb_state *mrb, mrb_value str, request_rec *r);


static int ap_mrb_push_request(request_rec *r)
{
    mrb_request_rec_state = (request_rec *)apr_pcalloc(r->pool, sizeof (*mrb_request_rec_state));
    mrb_request_rec_state = r;
    return OK;
}

request_rec *ap_mrb_get_request()
{
    return mrb_request_rec_state;
}

mrb_value
ap_mrb_rputs(mrb_state *mrb, mrb_value str)
{
    mrb_value msg;

    mrb_get_args(mrb, "o", &msg);

    ap_rputs(RSTRING_PTR(msg), ap_mrb_get_request()); 

    return str;
}

/*
mrb_value
ap_mrb_rputs_test(mrb_state *mrb, mrb_value str, request_rec *r) {

    ap_rputs("hello", r); 

    return mrb_nil_value();
}
*/

