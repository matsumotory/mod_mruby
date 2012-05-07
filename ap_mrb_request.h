#ifndef AP_MRB_REQUEST_H
#define AP_MRB_REQUEST_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "mruby.h"
#include "mruby/variable.h"

int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();
mrb_value ap_mrb_init_request(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_rec(mrb_state *mrb, const char *member);
mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_uri(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_write_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str);

#endif
