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

mrb_value ap_mrb_get_scoreboard_idle_worker(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_process_worker(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_restart_time(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_pid(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_server_limit(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_thread_limit(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_access_counter(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_uptime(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_total_kbyte(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_total_access(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_status(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_scoreboard_counter(mrb_state *mrb, mrb_value str);

#endif
