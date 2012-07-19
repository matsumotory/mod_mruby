#include "ap_mrb_init.h"
#include "ap_mrb_request.h"
#include "ap_mrb_server.h"
#include "ap_mrb_connection.h"
#include "ap_mrb_utils.h"
#include "ap_mrb_string.h"
#include "ap_mrb_scoreboard.h"

struct RClass *class;
struct RClass *class_request;
struct RClass *class_server;
struct RClass *class_conn;
struct RClass *class_headers_in;
struct RClass *class_headers_out;
struct RClass *class_scoreboard;

int ap_mruby_class_init(mrb_state *mrb)
{

    class = mrb_define_module(mrb, "Apache");
    mrb_define_const(mrb, class, "OK", mrb_fixnum_value(OK));
    mrb_define_const(mrb, class, "DECLINED", mrb_fixnum_value(DECLINED));
    mrb_define_const(mrb, class, "HTTP_SERVICE_UNAVAILABLE", mrb_fixnum_value(HTTP_SERVICE_UNAVAILABLE));
    mrb_define_const(mrb, class, "HTTP_CONTINUE", mrb_fixnum_value(HTTP_CONTINUE));
    mrb_define_const(mrb, class, "HTTP_SWITCHING_PROTOCOLS", mrb_fixnum_value(HTTP_SWITCHING_PROTOCOLS));
    mrb_define_const(mrb, class, "HTTP_PROCESSING", mrb_fixnum_value(HTTP_PROCESSING));
    mrb_define_const(mrb, class, "HTTP_OK", mrb_fixnum_value(HTTP_OK));
    mrb_define_const(mrb, class, "HTTP_CREATED", mrb_fixnum_value(HTTP_CREATED));
    mrb_define_const(mrb, class, "HTTP_ACCEPTED", mrb_fixnum_value(HTTP_ACCEPTED));
    mrb_define_const(mrb, class, "HTTP_NON_AUTHORITATIVE", mrb_fixnum_value(HTTP_NON_AUTHORITATIVE));
    mrb_define_const(mrb, class, "HTTP_NO_CONTENT", mrb_fixnum_value(HTTP_NO_CONTENT));
    mrb_define_const(mrb, class, "HTTP_RESET_CONTENT", mrb_fixnum_value(HTTP_RESET_CONTENT));
    mrb_define_const(mrb, class, "HTTP_PARTIAL_CONTENT", mrb_fixnum_value(HTTP_PARTIAL_CONTENT));
    mrb_define_const(mrb, class, "HTTP_MULTI_STATUS", mrb_fixnum_value(HTTP_MULTI_STATUS));
    mrb_define_const(mrb, class, "HTTP_MULTIPLE_CHOICES", mrb_fixnum_value(HTTP_MULTIPLE_CHOICES));
    mrb_define_const(mrb, class, "HTTP_MOVED_PERMANENTLY", mrb_fixnum_value(HTTP_MOVED_PERMANENTLY));
    mrb_define_const(mrb, class, "HTTP_MOVED_TEMPORARILY", mrb_fixnum_value(HTTP_MOVED_TEMPORARILY));
    mrb_define_const(mrb, class, "HTTP_SEE_OTHER", mrb_fixnum_value(HTTP_SEE_OTHER));
    mrb_define_const(mrb, class, "HTTP_NOT_MODIFIED", mrb_fixnum_value(HTTP_NOT_MODIFIED));
    mrb_define_const(mrb, class, "HTTP_USE_PROXY", mrb_fixnum_value(HTTP_USE_PROXY));
    mrb_define_const(mrb, class, "HTTP_TEMPORARY_REDIRECT", mrb_fixnum_value(HTTP_TEMPORARY_REDIRECT));
    mrb_define_const(mrb, class, "HTTP_BAD_REQUEST", mrb_fixnum_value(HTTP_BAD_REQUEST));
    mrb_define_const(mrb, class, "HTTP_UNAUTHORIZED", mrb_fixnum_value(HTTP_UNAUTHORIZED));
    mrb_define_const(mrb, class, "HTTP_PAYMENT_REQUIRED", mrb_fixnum_value(HTTP_PAYMENT_REQUIRED));
    mrb_define_const(mrb, class, "HTTP_FORBIDDEN", mrb_fixnum_value(HTTP_FORBIDDEN));
    mrb_define_const(mrb, class, "HTTP_NOT_FOUND", mrb_fixnum_value(HTTP_NOT_FOUND));
    mrb_define_const(mrb, class, "HTTP_METHOD_NOT_ALLOWED", mrb_fixnum_value(HTTP_METHOD_NOT_ALLOWED));
    mrb_define_const(mrb, class, "HTTP_NOT_ACCEPTABLE", mrb_fixnum_value(HTTP_NOT_ACCEPTABLE));
    mrb_define_const(mrb, class, "HTTP_PROXY_AUTHENTICATION_REQUIRED", mrb_fixnum_value(HTTP_PROXY_AUTHENTICATION_REQUIRED));
    mrb_define_const(mrb, class, "HTTP_REQUEST_TIME_OUT", mrb_fixnum_value(HTTP_REQUEST_TIME_OUT));
    mrb_define_const(mrb, class, "HTTP_CONFLICT", mrb_fixnum_value(HTTP_CONFLICT));
    mrb_define_const(mrb, class, "HTTP_GONE", mrb_fixnum_value(HTTP_GONE));
    mrb_define_const(mrb, class, "HTTP_LENGTH_REQUIRED", mrb_fixnum_value(HTTP_LENGTH_REQUIRED));
    mrb_define_const(mrb, class, "HTTP_PRECONDITION_FAILED", mrb_fixnum_value(HTTP_PRECONDITION_FAILED));
    mrb_define_const(mrb, class, "HTTP_REQUEST_ENTITY_TOO_LARGE", mrb_fixnum_value(HTTP_REQUEST_ENTITY_TOO_LARGE));
    mrb_define_const(mrb, class, "HTTP_REQUEST_URI_TOO_LARGE", mrb_fixnum_value(HTTP_REQUEST_URI_TOO_LARGE));
    mrb_define_const(mrb, class, "HTTP_UNSUPPORTED_MEDIA_TYPE", mrb_fixnum_value(HTTP_UNSUPPORTED_MEDIA_TYPE));
    mrb_define_const(mrb, class, "HTTP_RANGE_NOT_SATISFIABLE", mrb_fixnum_value(HTTP_RANGE_NOT_SATISFIABLE));
    mrb_define_const(mrb, class, "HTTP_EXPECTATION_FAILED", mrb_fixnum_value(HTTP_EXPECTATION_FAILED));
    mrb_define_const(mrb, class, "HTTP_UNPROCESSABLE_ENTITY", mrb_fixnum_value(HTTP_UNPROCESSABLE_ENTITY));
    mrb_define_const(mrb, class, "HTTP_LOCKED", mrb_fixnum_value(HTTP_LOCKED));
    mrb_define_const(mrb, class, "HTTP_NOT_EXTENDED", mrb_fixnum_value(HTTP_NOT_EXTENDED));
    mrb_define_const(mrb, class, "DOCUMENT_FOLLOWS", mrb_fixnum_value(HTTP_OK));
    mrb_define_const(mrb, class, "PARTIAL_CONTENT", mrb_fixnum_value(HTTP_PARTIAL_CONTENT));
    mrb_define_const(mrb, class, "MULTIPLE_CHOICES", mrb_fixnum_value(HTTP_MULTIPLE_CHOICES));
    mrb_define_const(mrb, class, "MOVED", mrb_fixnum_value(HTTP_MOVED_PERMANENTLY));
    mrb_define_const(mrb, class, "REDIRECT", mrb_fixnum_value(HTTP_MOVED_TEMPORARILY));
    mrb_define_const(mrb, class, "USE_LOCAL_COPY", mrb_fixnum_value(HTTP_NOT_MODIFIED));
    mrb_define_const(mrb, class, "BAD_REQUEST", mrb_fixnum_value(HTTP_BAD_REQUEST));
    mrb_define_const(mrb, class, "AUTH_REQUIRED", mrb_fixnum_value(HTTP_UNAUTHORIZED));
    mrb_define_const(mrb, class, "FORBIDDEN", mrb_fixnum_value(HTTP_FORBIDDEN));
    mrb_define_const(mrb, class, "NOT_FOUND", mrb_fixnum_value(HTTP_NOT_FOUND));
    mrb_define_const(mrb, class, "METHOD_NOT_ALLOWED", mrb_fixnum_value(HTTP_METHOD_NOT_ALLOWED));
    mrb_define_const(mrb, class, "NOT_ACCEPTABLE", mrb_fixnum_value(HTTP_NOT_ACCEPTABLE));
    mrb_define_const(mrb, class, "LENGTH_REQUIRED", mrb_fixnum_value(HTTP_LENGTH_REQUIRED));
    mrb_define_const(mrb, class, "PRECONDITION_FAILED", mrb_fixnum_value(HTTP_PRECONDITION_FAILED));
    mrb_define_const(mrb, class, "SERVER_ERROR", mrb_fixnum_value(HTTP_INTERNAL_SERVER_ERROR));
    mrb_define_const(mrb, class, "NOT_IMPLEMENTED", mrb_fixnum_value(HTTP_NOT_IMPLEMENTED));
    mrb_define_const(mrb, class, "BAD_GATEWAY", mrb_fixnum_value(HTTP_BAD_GATEWAY));
    mrb_define_const(mrb, class, "VARIANT_ALSO_VARIES", mrb_fixnum_value(HTTP_VARIANT_ALSO_VARIES));
    mrb_define_const(mrb, class, "PROXYREQ_NONE", mrb_fixnum_value(PROXYREQ_NONE));
    mrb_define_const(mrb, class, "PROXYREQ_PROXY", mrb_fixnum_value(PROXYREQ_PROXY));
    mrb_define_const(mrb, class, "PROXYREQ_REVERSE", mrb_fixnum_value(PROXYREQ_REVERSE));
    mrb_define_const(mrb, class, "PROXYREQ_RESPONSE", mrb_fixnum_value(PROXYREQ_RESPONSE));
    mrb_define_class_method(mrb, class, "sleep", ap_mrb_sleep, ARGS_ANY());
    mrb_define_class_method(mrb, class, "rputs", ap_mrb_rputs, ARGS_ANY());
    mrb_define_class_method(mrb, class, "return", ap_mrb_return, ARGS_ANY());
    mrb_define_class_method(mrb, class, "errlogger", ap_mrb_errlogger, ARGS_ANY());
    mrb_define_class_method(mrb, class, "syslogger", ap_mrb_syslogger, ARGS_ANY());
    mrb_define_class_method(mrb, class, "write_request", ap_mrb_write_request, ARGS_ANY());
    mrb_define_class_method(mrb, class, "mod_mruby_version", ap_mrb_get_mod_mruby_version, ARGS_NONE());
    mrb_define_class_method(mrb, class, "apache_version", ap_mrb_get_apache_version, ARGS_NONE());

    class_server = mrb_define_class_under(mrb, class, "Server", mrb->object_class);
    mrb_define_method(mrb, class_server, "error_fname=", ap_mrb_set_server_error_fname, ARGS_ANY());
    mrb_define_method(mrb, class_server, "error_fname", ap_mrb_get_server_error_fname, ARGS_NONE());
    mrb_define_method(mrb, class_server, "document_root", ap_mrb_get_server_document_root, ARGS_NONE());
    mrb_define_method(mrb, class_server, "loglevel=", ap_mrb_set_server_loglevel, ARGS_ANY());
    mrb_define_method(mrb, class_server, "loglevel", ap_mrb_get_server_loglevel, ARGS_NONE());
    mrb_define_method(mrb, class_server, "hostname", ap_mrb_get_server_hostname, ARGS_NONE());
    mrb_define_method(mrb, class_server, "path", ap_mrb_get_server_path, ARGS_NONE());
    mrb_define_method(mrb, class_server, "admin", ap_mrb_get_server_admin, ARGS_NONE());
    mrb_define_method(mrb, class_server, "scheme", ap_mrb_get_server_scheme, ARGS_NONE());
    mrb_define_method(mrb, class_server, "defn_name", ap_mrb_get_server_defn_name, ARGS_NONE());
    mrb_define_method(mrb, class_server, "is_virtual", ap_mrb_get_server_is_virtual, ARGS_NONE());
    mrb_define_method(mrb, class_server, "keep_alive_max", ap_mrb_get_server_keep_alive_max, ARGS_NONE());
    mrb_define_method(mrb, class_server, "keep_alive", ap_mrb_get_server_keep_alive, ARGS_NONE());
    mrb_define_method(mrb, class_server, "pathlen", ap_mrb_get_server_pathlen, ARGS_NONE());
    mrb_define_method(mrb, class_server, "limit_req_line", ap_mrb_get_server_limit_req_line, ARGS_NONE());
    mrb_define_method(mrb, class_server, "limit_req_fieldsize", ap_mrb_get_server_limit_req_fieldsize, ARGS_NONE());
    mrb_define_method(mrb, class_server, "limit_req_fields", ap_mrb_get_server_limit_req_fields, ARGS_NONE());
    mrb_define_method(mrb, class_server, "timeout", ap_mrb_get_server_timeout, ARGS_NONE());
    mrb_define_method(mrb, class_server, "keep_alive_timeout", ap_mrb_get_server_keep_alive_timeout, ARGS_NONE());
    mrb_define_method(mrb, class_server, "port", ap_mrb_get_server_port, ARGS_NONE());
    mrb_define_method(mrb, class_server, "defn_line_number", ap_mrb_get_server_defn_line_number, ARGS_NONE());

    class_scoreboard = mrb_define_class_under(mrb, class, "Scoreboard", mrb->object_class);
    mrb_define_method(mrb, class_scoreboard, "status", ap_mrb_get_scoreboard_status, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "counter", ap_mrb_get_scoreboard_counter, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "pid", ap_mrb_get_scoreboard_pid, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "server_limit", ap_mrb_get_scoreboard_server_limit, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "thread_limit", ap_mrb_get_scoreboard_thread_limit, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "access_counter", ap_mrb_get_scoreboard_access_counter, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "total_kbyte", ap_mrb_get_scoreboard_total_kbyte, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "total_access", ap_mrb_get_scoreboard_total_access, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "uptime", ap_mrb_get_scoreboard_uptime, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "restart_time", ap_mrb_get_scoreboard_restart_time, ARGS_ANY());

    class_conn = mrb_define_class_under(mrb, class, "Connection", mrb->object_class);
    mrb_define_method(mrb, class_conn, "remote_ip", ap_mrb_get_conn_remote_ip, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "remote_host", ap_mrb_get_conn_remote_host, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "remote_logname", ap_mrb_get_conn_remote_logname, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "local_ip", ap_mrb_get_conn_local_ip, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "local_host", ap_mrb_get_conn_local_host, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "keepalives", ap_mrb_get_conn_keepalives, ARGS_NONE());
    mrb_define_method(mrb, class_conn, "data_in_input_filters", ap_mrb_get_conn_data_in_input_filters, ARGS_NONE());

    class_request = mrb_define_class_under(mrb, class, "Request", mrb->object_class);
    //mrb_define_method(mrb, class_request, "Initialize", ap_mrb_init_request, ARGS_NONE());
    //mrb_define_method(mrb, class_request, "request_rec_json", ap_mrb_get_request_rec_json, ARGS_NONE());
    mrb_define_method(mrb, class_request, "the_request=", ap_mrb_set_request_the_request, ARGS_ANY());
    mrb_define_method(mrb, class_request, "the_request", ap_mrb_get_request_the_request, ARGS_NONE());
    mrb_define_method(mrb, class_request, "protocol=", ap_mrb_set_request_protocol, ARGS_ANY());
    mrb_define_method(mrb, class_request, "protocol", ap_mrb_get_request_protocol, ARGS_NONE());
    mrb_define_method(mrb, class_request, "vlist_validator=", ap_mrb_set_request_vlist_validator, ARGS_ANY());
    mrb_define_method(mrb, class_request, "vlist_validator", ap_mrb_get_request_vlist_validator, ARGS_NONE());
    mrb_define_method(mrb, class_request, "user=", ap_mrb_set_request_user, ARGS_ANY());
    mrb_define_method(mrb, class_request, "user", ap_mrb_get_request_user, ARGS_NONE());
    mrb_define_method(mrb, class_request, "ap_auth_type=", ap_mrb_set_request_ap_auth_type, ARGS_ANY());
    mrb_define_method(mrb, class_request, "ap_auth_type", ap_mrb_get_request_ap_auth_type, ARGS_NONE());
    mrb_define_method(mrb, class_request, "unparsed_uri=", ap_mrb_set_request_unparsed_uri, ARGS_ANY());
    mrb_define_method(mrb, class_request, "unparsed_uri", ap_mrb_get_request_unparsed_uri, ARGS_NONE());
    mrb_define_method(mrb, class_request, "uri=", ap_mrb_set_request_uri, ARGS_ANY());
    mrb_define_method(mrb, class_request, "uri", ap_mrb_get_request_uri, ARGS_NONE());
    mrb_define_method(mrb, class_request, "filename=", ap_mrb_set_request_filename, ARGS_ANY());
    mrb_define_method(mrb, class_request, "filename", ap_mrb_get_request_filename, ARGS_NONE());
    mrb_define_method(mrb, class_request, "canonical_filename=", ap_mrb_set_request_canonical_filename, ARGS_ANY());
    mrb_define_method(mrb, class_request, "canonical_filename", ap_mrb_get_request_canonical_filename, ARGS_NONE());
    mrb_define_method(mrb, class_request, "path_info=", ap_mrb_set_request_path_info, ARGS_ANY());
    mrb_define_method(mrb, class_request, "path_info", ap_mrb_get_request_path_info, ARGS_NONE());
    mrb_define_method(mrb, class_request, "args=", ap_mrb_set_request_args, ARGS_ANY());
    mrb_define_method(mrb, class_request, "args", ap_mrb_get_request_args, ARGS_NONE());

    mrb_define_method(mrb, class_request, "hostname=", ap_mrb_set_request_hostname, ARGS_ANY());
    mrb_define_method(mrb, class_request, "hostname", ap_mrb_get_request_hostname, ARGS_NONE());
    mrb_define_method(mrb, class_request, "status_line=", ap_mrb_set_request_status_line, ARGS_ANY());
    mrb_define_method(mrb, class_request, "status_line", ap_mrb_get_request_status_line, ARGS_NONE());
    mrb_define_method(mrb, class_request, "method=", ap_mrb_set_request_method, ARGS_ANY());
    mrb_define_method(mrb, class_request, "method", ap_mrb_get_request_method, ARGS_NONE());
    mrb_define_method(mrb, class_request, "range=", ap_mrb_set_request_range, ARGS_ANY());
    mrb_define_method(mrb, class_request, "range", ap_mrb_get_request_range, ARGS_NONE());
    mrb_define_method(mrb, class_request, "content_type=", ap_mrb_set_request_content_type, ARGS_ANY());
    mrb_define_method(mrb, class_request, "content_type", ap_mrb_get_request_content_type, ARGS_NONE());
    mrb_define_method(mrb, class_request, "handler=", ap_mrb_set_request_handler, ARGS_ANY());
    mrb_define_method(mrb, class_request, "handler", ap_mrb_get_request_handler, ARGS_NONE());
    mrb_define_method(mrb, class_request, "content_encoding=", ap_mrb_set_request_content_encoding, ARGS_ANY());
    mrb_define_method(mrb, class_request, "content_encoding", ap_mrb_get_request_content_encoding, ARGS_NONE());

    //class_headers_in = mrb_define_class(mrb, "headers_in", class_request);
    class_headers_in = mrb_define_class_under(mrb, class, "Headers_in", mrb->object_class);
    mrb_define_method(mrb, class_headers_in, "[]=", ap_mrb_set_request_headers_in, ARGS_ANY());
    mrb_define_method(mrb, class_headers_in, "[]", ap_mrb_get_request_headers_in, ARGS_ANY());
    mrb_define_method(mrb, class_headers_in, "headers_in_hash", ap_mrb_get_request_headers_in_hash, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "[]=", ap_mrb_set_request_readers_in, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "[]", ap_mrb_get_request_readers_in, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "headers_in=", ap_mrb_set_request_readers_in, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "headers_in", ap_mrb_get_request_readers_in, ARGS_ANY());
    class_headers_out = mrb_define_class_under(mrb, class, "Headers_out", mrb->object_class);
    mrb_define_method(mrb, class_headers_out, "headers_out=", ap_mrb_set_request_headers_out, ARGS_ANY());
    mrb_define_method(mrb, class_headers_out, "headers_out", ap_mrb_get_request_headers_out, ARGS_ANY());
    mrb_define_method(mrb, class_headers_out, "headers_out_hash", ap_mrb_get_request_headers_out_hash, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "headers_out=", ap_mrb_set_request_readers_out, ARGS_ANY());
    //mrb_define_method(mrb, class_request, "headers_out", ap_mrb_get_request_readers_out, ARGS_ANY());

    mrb_define_method(mrb, class_request, "assbackwards", ap_mrb_get_request_assbackwards, ARGS_NONE());
    mrb_define_method(mrb, class_request, "proxyreq=", ap_mrb_set_request_proxyreq, ARGS_ANY());
    mrb_define_method(mrb, class_request, "proxyreq", ap_mrb_get_request_proxyreq, ARGS_NONE());
    mrb_define_method(mrb, class_request, "header_only", ap_mrb_get_request_header_only, ARGS_NONE());
    mrb_define_method(mrb, class_request, "proto_num", ap_mrb_get_request_proto_num, ARGS_NONE());
    mrb_define_method(mrb, class_request, "status", ap_mrb_get_request_status, ARGS_NONE());
    mrb_define_method(mrb, class_request, "method_number", ap_mrb_get_request_method_number, ARGS_NONE());
    mrb_define_method(mrb, class_request, "chunked", ap_mrb_get_request_chunked, ARGS_NONE());
    mrb_define_method(mrb, class_request, "read_body", ap_mrb_get_request_read_body, ARGS_NONE());
    mrb_define_method(mrb, class_request, "read_chunked", ap_mrb_get_request_read_chunked, ARGS_NONE());
    mrb_define_method(mrb, class_request, "used_path_info", ap_mrb_get_request_used_path_info, ARGS_NONE());
    mrb_define_method(mrb, class_request, "eos_sent", ap_mrb_get_request_eos_sent, ARGS_NONE());
    mrb_define_method(mrb, class_request, "no_cache", ap_mrb_get_request_no_cache, ARGS_NONE());
    mrb_define_method(mrb, class_request, "no_local_copy", ap_mrb_get_request_no_local_copy, ARGS_NONE());


    return OK;
}
