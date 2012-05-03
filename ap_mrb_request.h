#ifndef AP_MRB_REQUEST_H
#define AP_MRB_REQUEST_H

#include <string.h>
#include "mruby/string.h"


int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();
mrb_value ap_mrb_request(mrb_state *mrb, mrb_value str);


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

#endif
