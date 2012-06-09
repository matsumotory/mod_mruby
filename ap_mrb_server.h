#ifndef AP_MRB_SERVER_H
#define AP_MRB_SERVER_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "ap_mrb_request.h"


mrb_value ap_mrb_set_server_error_fname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_error_fname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_document_root(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_server_loglevel(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_loglevel(mrb_state *mrb, mrb_value str);

#endif
