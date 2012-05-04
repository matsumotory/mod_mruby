#ifndef AP_MRB_REQUEST_H
#define AP_MRB_REQUEST_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "mruby.h"

int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();
mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str);

#endif
