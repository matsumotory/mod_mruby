#ifndef AP_MRB_REDIS_H
#define AP_MRB_REDIS_H

#include "ap_mrb_request.h"
#include "http_protocol.h"
#include "mruby/string.h"
#include "hiredis/hiredis.h"

mrb_value ap_mrb_redis_connect(mrb_state *mrb, mrb_value self);
mrb_value ap_mrb_redis_set(mrb_state *mrb, mrb_value self);
mrb_value ap_mrb_redis_get(mrb_state *mrb, mrb_value self);
//mrb_value ap_mrb_redis_mget(mrb_state *mrb, mrb_value self);

#endif
