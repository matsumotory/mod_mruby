#ifndef AP_MRB_ENV_H
#define AP_MRB_ENV_H

#include "ap_mrb_request.h"
#include "mruby/string.h"

mrb_value ap_mrb_init_env(mrb_state *mrb, mrb_value self);
mrb_value ap_mrb_set_env(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_env(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_env_hash(mrb_state *mrb, mrb_value str);

#endif
