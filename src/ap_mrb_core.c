/*
// ap_mrb_core.c - to proveid mod_mruby core
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_core.h"
#include "ap_mrb_request.h"

#ifndef _WIN32
#define SUPPORT_SYSLOG
#else
#define sleep(x) Sleep(x*1000)
#endif

#ifdef SUPPORT_SYSLOG
CODE logpriority[] =
{
    { "alert", LOG_ALERT },
    { "crit", LOG_CRIT },
    { "debug", LOG_DEBUG },
    { "emerg", LOG_EMERG },
    { "err", LOG_ERR },
    { "error", LOG_ERR },
    { "info", LOG_INFO },
    { "notice", LOG_NOTICE },
    { "panic", LOG_EMERG },
    { "warn", LOG_WARNING },
    { "warning", LOG_WARNING },
    { NULL, -1 }
};
#endif

int mod_mruby_return_code;

int ap_mrb_get_status_code()
{
    return mod_mruby_return_code;
}


int ap_mrb_set_status_code(int val)
{
    mod_mruby_return_code = val;
    return 0;
}

void ap_mrb_raise_file_error(mrb_state *mrb, mrb_value obj, request_rec *r, const char *file)
{
   struct RString *str;
   char *err_out;

   obj = mrb_funcall(mrb, obj, "inspect", 0);

   if (mrb_type(obj) == MRB_TT_STRING) {
       str = mrb_str_ptr(obj);
       err_out = str->ptr;
       ap_log_error(APLOG_MARK
           , APLOG_ERR
           , 0
           , NULL 
           , "%s ERROR %s: mrb_run failed. file: %s error: %s"
           , MODULE_NAME
           , __func__
           , file
           , err_out
       );
   }
}

void ap_mrb_raise_file_error_nr(mrb_state *mrb, mrb_value obj, const char *file)
{
   struct RString *str;
   char *err_out;

   obj = mrb_funcall(mrb, obj, "inspect", 0);

   if (mrb_type(obj) == MRB_TT_STRING) {
       str = mrb_str_ptr(obj);
       err_out = str->ptr;
       ap_log_error(APLOG_MARK
           , APLOG_ERR
           , 0
           , NULL 
           , "%s ERROR %s: mrb_run failed. file: %s error: %s"
           , MODULE_NAME
           , __func__
           , file
           , err_out
       );
   }
}

void ap_mrb_raise_error(mrb_state *mrb, mrb_value obj, request_rec *r)
{
   struct RString *str;
   char *err_out;

   obj = mrb_funcall(mrb, obj, "inspect", 0);

   if (mrb_type(obj) == MRB_TT_STRING) {
       str = mrb_str_ptr(obj);
       err_out = str->ptr;
       ap_log_error(APLOG_MARK
           , APLOG_ERR
           , 0
           , NULL
           , "%s ERROR %s: mrb_run failed. error: %s"
           , MODULE_NAME
           , __func__
           , err_out
       );
   }
}

mrb_value ap_mrb_return(mrb_state *mrb, mrb_value self)
{

    mrb_int ret;

    mrb_get_args(mrb, "i", &ret);
    ap_mrb_set_status_code((int)ret);

    return self;
}

mrb_value ap_mrb_get_mod_mruby_version(mrb_state *mrb, mrb_value str)
{
    return mrb_str_new(mrb, MODULE_VERSION, strlen(MODULE_VERSION));
}


mrb_value ap_mrb_get_server_version(mrb_state *mrb, mrb_value str)
{
#if AP_SERVER_PATCHLEVEL_NUMBER > 3
    return mrb_str_new(mrb, ap_get_server_description(), strlen(ap_get_server_description()));
#else
    return mrb_str_new_cstr(mrb, AP_SERVER_BASEVERSION " (" PLATFORM ")");
#endif
}

mrb_value ap_mrb_get_server_build(mrb_state *mrb, mrb_value str)
{
    return mrb_str_new(mrb, ap_get_server_built(), strlen(ap_get_server_built()));
}

mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str)
{

    mrb_int time;
    
    mrb_get_args(mrb, "i", &time);
    sleep((int)time);

    return str;
}

mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str)
{

    mrb_value *argv;
    mrb_int argc;
    
    mrb_get_args(mrb, "*", &argv, &argc);
    if (argc != 2) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 2"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    ap_log_error(APLOG_MARK, mrb_fixnum(argv[0]), 0, NULL, "%s", RSTRING_PTR(argv[1]));

    return str;
}


mrb_value ap_mrb_syslogger(mrb_state *mrb, mrb_value str)
{   

#ifdef SUPPORT_SYSLOG
    mrb_value *argv;
    mrb_int argc;
    char *i_pri,*msg;
    int i;
    int pri = INVALID_PRIORITY;

    mrb_get_args(mrb, "*", &argv, &argc);
    if (argc != 2) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 2"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    i_pri = RSTRING_PTR(argv[0]);

    i = 0;
    while (logpriority[i].c_name != NULL) {
        if (strcmp(logpriority[i].c_name, i_pri) == 0) {
            pri = logpriority[i].c_val;
            break;
        }
        i++;
    }

    if (pri == INVALID_PRIORITY) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: priority is invalid"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    msg = RSTRING_PTR(argv[1]);

    openlog(NULL, LOG_PID, LOG_SYSLOG);
    syslog(pri, "%s", msg);
    closelog();
#endif

    return str;
}

mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str)
{
    mrb_value msg;

    mrb_get_args(mrb, "o", &msg);
    if (mrb_type(msg) == MRB_TT_STRING) {
        ap_rputs(RSTRING_PTR(msg), ap_mrb_get_request());
    } else {
        ap_rputs("not string", ap_mrb_get_request());
    }

    return str;
}

mrb_value ap_mrb_server_name(mrb_state *mrb, mrb_value self)
{
    return mrb_str_new_cstr(mrb, AP_SERVER_BASEPRODUCT);
}

mrb_value ap_mrb_f_global_remove(mrb_state *mrb, mrb_value self)
{
    mrb_sym id;
    mrb_get_args(mrb, "n", &id);
    mrb_gv_remove(mrb, id);

    return mrb_f_global_variables(mrb, self);
}

void ap_mruby_core_init(mrb_state *mrb, struct RClass *class_core)
{

    mrb_define_method(mrb, mrb->kernel_module, "server_name", ap_mrb_server_name, ARGS_NONE());

    mrb_define_const(mrb, class_core, "OK", mrb_fixnum_value(OK));
    mrb_define_const(mrb, class_core, "DECLINED", mrb_fixnum_value(DECLINED));
    mrb_define_const(mrb, class_core, "HTTP_SERVICE_UNAVAILABLE", mrb_fixnum_value(HTTP_SERVICE_UNAVAILABLE));
    mrb_define_const(mrb, class_core, "HTTP_CONTINUE", mrb_fixnum_value(HTTP_CONTINUE));
    mrb_define_const(mrb, class_core, "HTTP_SWITCHING_PROTOCOLS", mrb_fixnum_value(HTTP_SWITCHING_PROTOCOLS));
    mrb_define_const(mrb, class_core, "HTTP_PROCESSING", mrb_fixnum_value(HTTP_PROCESSING));
    mrb_define_const(mrb, class_core, "HTTP_OK", mrb_fixnum_value(HTTP_OK));
    mrb_define_const(mrb, class_core, "HTTP_CREATED", mrb_fixnum_value(HTTP_CREATED));
    mrb_define_const(mrb, class_core, "HTTP_ACCEPTED", mrb_fixnum_value(HTTP_ACCEPTED));
    mrb_define_const(mrb, class_core, "HTTP_NON_AUTHORITATIVE", mrb_fixnum_value(HTTP_NON_AUTHORITATIVE));
    mrb_define_const(mrb, class_core, "HTTP_NO_CONTENT", mrb_fixnum_value(HTTP_NO_CONTENT));
    mrb_define_const(mrb, class_core, "HTTP_RESET_CONTENT", mrb_fixnum_value(HTTP_RESET_CONTENT));
    mrb_define_const(mrb, class_core, "HTTP_PARTIAL_CONTENT", mrb_fixnum_value(HTTP_PARTIAL_CONTENT));
    mrb_define_const(mrb, class_core, "HTTP_MULTI_STATUS", mrb_fixnum_value(HTTP_MULTI_STATUS));
    mrb_define_const(mrb, class_core, "HTTP_MULTIPLE_CHOICES", mrb_fixnum_value(HTTP_MULTIPLE_CHOICES));
    mrb_define_const(mrb, class_core, "HTTP_MOVED_PERMANENTLY", mrb_fixnum_value(HTTP_MOVED_PERMANENTLY));
    mrb_define_const(mrb, class_core, "HTTP_MOVED_TEMPORARILY", mrb_fixnum_value(HTTP_MOVED_TEMPORARILY));
    mrb_define_const(mrb, class_core, "HTTP_SEE_OTHER", mrb_fixnum_value(HTTP_SEE_OTHER));
    mrb_define_const(mrb, class_core, "HTTP_NOT_MODIFIED", mrb_fixnum_value(HTTP_NOT_MODIFIED));
    mrb_define_const(mrb, class_core, "HTTP_USE_PROXY", mrb_fixnum_value(HTTP_USE_PROXY));
    mrb_define_const(mrb, class_core, "HTTP_TEMPORARY_REDIRECT", mrb_fixnum_value(HTTP_TEMPORARY_REDIRECT));
    mrb_define_const(mrb, class_core, "HTTP_BAD_REQUEST", mrb_fixnum_value(HTTP_BAD_REQUEST));
    mrb_define_const(mrb, class_core, "HTTP_UNAUTHORIZED", mrb_fixnum_value(HTTP_UNAUTHORIZED));
    mrb_define_const(mrb, class_core, "HTTP_PAYMENT_REQUIRED", mrb_fixnum_value(HTTP_PAYMENT_REQUIRED));
    mrb_define_const(mrb, class_core, "HTTP_FORBIDDEN", mrb_fixnum_value(HTTP_FORBIDDEN));
    mrb_define_const(mrb, class_core, "HTTP_NOT_FOUND", mrb_fixnum_value(HTTP_NOT_FOUND));
    mrb_define_const(mrb, class_core, "HTTP_METHOD_NOT_ALLOWED", mrb_fixnum_value(HTTP_METHOD_NOT_ALLOWED));
    mrb_define_const(mrb, class_core, "HTTP_NOT_ACCEPTABLE", mrb_fixnum_value(HTTP_NOT_ACCEPTABLE));
    mrb_define_const(mrb, class_core, "HTTP_PROXY_AUTHENTICATION_REQUIRED", mrb_fixnum_value(HTTP_PROXY_AUTHENTICATION_REQUIRED));
    mrb_define_const(mrb, class_core, "HTTP_REQUEST_TIME_OUT", mrb_fixnum_value(HTTP_REQUEST_TIME_OUT));
    mrb_define_const(mrb, class_core, "HTTP_CONFLICT", mrb_fixnum_value(HTTP_CONFLICT));
    mrb_define_const(mrb, class_core, "HTTP_GONE", mrb_fixnum_value(HTTP_GONE));
    mrb_define_const(mrb, class_core, "HTTP_LENGTH_REQUIRED", mrb_fixnum_value(HTTP_LENGTH_REQUIRED));
    mrb_define_const(mrb, class_core, "HTTP_PRECONDITION_FAILED", mrb_fixnum_value(HTTP_PRECONDITION_FAILED));
    mrb_define_const(mrb, class_core, "HTTP_REQUEST_ENTITY_TOO_LARGE", mrb_fixnum_value(HTTP_REQUEST_ENTITY_TOO_LARGE));
    mrb_define_const(mrb, class_core, "HTTP_REQUEST_URI_TOO_LARGE", mrb_fixnum_value(HTTP_REQUEST_URI_TOO_LARGE));
    mrb_define_const(mrb, class_core, "HTTP_UNSUPPORTED_MEDIA_TYPE", mrb_fixnum_value(HTTP_UNSUPPORTED_MEDIA_TYPE));
    mrb_define_const(mrb, class_core, "HTTP_RANGE_NOT_SATISFIABLE", mrb_fixnum_value(HTTP_RANGE_NOT_SATISFIABLE));
    mrb_define_const(mrb, class_core, "HTTP_EXPECTATION_FAILED", mrb_fixnum_value(HTTP_EXPECTATION_FAILED));
    mrb_define_const(mrb, class_core, "HTTP_UNPROCESSABLE_ENTITY", mrb_fixnum_value(HTTP_UNPROCESSABLE_ENTITY));
    mrb_define_const(mrb, class_core, "HTTP_LOCKED", mrb_fixnum_value(HTTP_LOCKED));
    mrb_define_const(mrb, class_core, "HTTP_NOT_EXTENDED", mrb_fixnum_value(HTTP_NOT_EXTENDED));
    mrb_define_const(mrb, class_core, "DOCUMENT_FOLLOWS", mrb_fixnum_value(HTTP_OK));
    mrb_define_const(mrb, class_core, "PARTIAL_CONTENT", mrb_fixnum_value(HTTP_PARTIAL_CONTENT));
    mrb_define_const(mrb, class_core, "MULTIPLE_CHOICES", mrb_fixnum_value(HTTP_MULTIPLE_CHOICES));
    mrb_define_const(mrb, class_core, "MOVED", mrb_fixnum_value(HTTP_MOVED_PERMANENTLY));
    mrb_define_const(mrb, class_core, "REDIRECT", mrb_fixnum_value(HTTP_MOVED_TEMPORARILY));
    mrb_define_const(mrb, class_core, "USE_LOCAL_COPY", mrb_fixnum_value(HTTP_NOT_MODIFIED));
    mrb_define_const(mrb, class_core, "BAD_REQUEST", mrb_fixnum_value(HTTP_BAD_REQUEST));
    mrb_define_const(mrb, class_core, "AUTH_REQUIRED", mrb_fixnum_value(HTTP_UNAUTHORIZED));
    mrb_define_const(mrb, class_core, "FORBIDDEN", mrb_fixnum_value(HTTP_FORBIDDEN));
    mrb_define_const(mrb, class_core, "NOT_FOUND", mrb_fixnum_value(HTTP_NOT_FOUND));
    mrb_define_const(mrb, class_core, "METHOD_NOT_ALLOWED", mrb_fixnum_value(HTTP_METHOD_NOT_ALLOWED));
    mrb_define_const(mrb, class_core, "NOT_ACCEPTABLE", mrb_fixnum_value(HTTP_NOT_ACCEPTABLE));
    mrb_define_const(mrb, class_core, "LENGTH_REQUIRED", mrb_fixnum_value(HTTP_LENGTH_REQUIRED));
    mrb_define_const(mrb, class_core, "PRECONDITION_FAILED", mrb_fixnum_value(HTTP_PRECONDITION_FAILED));
    mrb_define_const(mrb, class_core, "SERVER_ERROR", mrb_fixnum_value(HTTP_INTERNAL_SERVER_ERROR));
    mrb_define_const(mrb, class_core, "NOT_IMPLEMENTED", mrb_fixnum_value(HTTP_NOT_IMPLEMENTED));
    mrb_define_const(mrb, class_core, "BAD_GATEWAY", mrb_fixnum_value(HTTP_BAD_GATEWAY));
    mrb_define_const(mrb, class_core, "VARIANT_ALSO_VARIES", mrb_fixnum_value(HTTP_VARIANT_ALSO_VARIES));
    mrb_define_const(mrb, class_core, "PROXYREQ_NONE", mrb_fixnum_value(PROXYREQ_NONE));
    mrb_define_const(mrb, class_core, "PROXYREQ_PROXY", mrb_fixnum_value(PROXYREQ_PROXY));
    mrb_define_const(mrb, class_core, "PROXYREQ_REVERSE", mrb_fixnum_value(PROXYREQ_REVERSE));
    mrb_define_const(mrb, class_core, "PROXYREQ_RESPONSE", mrb_fixnum_value(PROXYREQ_RESPONSE));

    mrb_define_class_method(mrb, class_core, "sleep", ap_mrb_sleep, ARGS_ANY());
    mrb_define_class_method(mrb, class_core, "rputs", ap_mrb_rputs, ARGS_ANY());
    mrb_define_class_method(mrb, class_core, "return", ap_mrb_return, ARGS_ANY());
    mrb_define_class_method(mrb, class_core, "errlogger", ap_mrb_errlogger, ARGS_ANY());
    mrb_define_class_method(mrb, class_core, "syslogger", ap_mrb_syslogger, ARGS_ANY());
    //mrb_define_class_method(mrb, class, "write_request", ap_mrb_write_request, ARGS_ANY());
    mrb_define_class_method(mrb, class_core, "mod_mruby_version", ap_mrb_get_mod_mruby_version, ARGS_NONE());
    mrb_define_class_method(mrb, class_core, "server_version", ap_mrb_get_server_version, ARGS_NONE());
    mrb_define_class_method(mrb, class_core, "server_build", ap_mrb_get_server_build, ARGS_NONE());
    mrb_define_class_method(mrb, class_core, "remove_global_variable", ap_mrb_f_global_remove, ARGS_REQ(1));
}
