#include "mod_mruby.h"
#include "ap_mrb_utils.h"

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

mrb_value ap_mrb_get_apache_version(mrb_state *mrb, mrb_value str)
{
    return mrb_str_new(mrb, AP_SERVER_BASEREVISION, strlen(AP_SERVER_BASEREVISION));
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
