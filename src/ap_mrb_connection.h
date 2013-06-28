#ifndef AP_MRB_CONNECTION_H
#define AP_MRB_CONNECTION_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "http_core.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "ap_mrb_request.h"

mrb_value ap_mrb_get_conn_remote_ip(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_remote_host(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_remote_logname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_local_ip(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_local_host(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_keepalives(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_conn_data_in_input_filters(mrb_state *mrb, mrb_value str);

#endif
