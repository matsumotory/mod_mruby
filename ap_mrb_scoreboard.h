#ifndef AP_MRB_SCOREBOARD_H
#define AP_MRB_SCOREBOARD_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "http_core.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "ap_mrb_request.h"

mrb_value ap_mrb_get_scoreboard_status(mrb_state *mrb, mrb_value str);

#endif
