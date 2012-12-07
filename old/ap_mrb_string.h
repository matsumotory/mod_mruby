#ifndef AP_MRB_STRING_H
#define AP_MRB_STRING_H

#include "ap_mrb_request.h"
#include "http_protocol.h"
#include "mruby/string.h"

mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str);

#endif
