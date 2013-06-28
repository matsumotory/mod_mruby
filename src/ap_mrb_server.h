#ifndef AP_MRB_SERVER_H
#define AP_MRB_SERVER_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "http_core.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "ap_mrb_request.h"


mrb_value ap_mrb_set_server_error_fname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_error_fname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_document_root(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_hostname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_path(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_admin(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_scheme(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_defn_name(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_server_loglevel(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_loglevel(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_is_virtual(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_keep_alive_max(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_keep_alive(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_pathlen(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_limit_req_line(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_limit_req_fieldsize(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_limit_req_fields(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_timeout(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_keep_alive_timeout(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_port(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_server_defn_line_number(mrb_state *mrb, mrb_value str);

#endif
