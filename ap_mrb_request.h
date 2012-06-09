#ifndef AP_MRB_REQUEST_H
#define AP_MRB_REQUEST_H

#include <string.h>
#include "apr_strings.h"
#include "http_request.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/class.h"

const char *ap_mrb_string_check(apr_pool_t *p, const char *str);
int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();

mrb_value ap_mrb_init_request(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_rec_json(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_get_request_the_request(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_protocol(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_vlist_validator(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_user(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_ap_auth_type(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_unparsed_uri(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_uri(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_canonical_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_path_info(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_args(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_get_request_hostname(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_status_line(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_method(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_range(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_content_type(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_handler(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_content_encoding(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_set_request_the_request(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_protocol(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_vlist_validator(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_user(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_ap_auth_type(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_unparsed_uri(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_uri(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_canonical_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_path_info(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_args(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_set_request_content_type(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_set_request_handler(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_get_request_assbackwards(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_proxyreq(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_header_only(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_proto_num(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_status(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_method_number(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_chunked(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_read_body(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_read_chunked(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_used_path_info(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_eos_sent(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_no_cache(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_request_no_local_copy(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_write_request_filename(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str);


#endif
