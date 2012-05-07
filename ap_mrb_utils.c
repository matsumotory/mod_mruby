#include "mod_mruby.h"
#include "ap_mrb_utils.h"

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


int ap_mrb_get_status_code()
{
    return mod_mruby_return_code;
}

int ap_mrb_set_status_code(mrb_value val)
{
    mod_mruby_return_code = mrb_fixnum(val);
    return 0;
}

mrb_value ap_mrb_return(mrb_state *mrb, mrb_value self)
{

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 1) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 1"
            , MODULE_NAME
            , __func__
        );
        return self;
    }
    ap_mrb_set_status_code(argv[0]);

    return self;
}


mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str)
{

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 1) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 1"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    sleep(mrb_fixnum(argv[0]));

    return str;
}

mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str)
{

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
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

    ap_log_error(APLOG_MARK, mrb_fixnum(argv[0]), 0, NULL, RSTRING_PTR(argv[1]));

    return str;
}

mrb_value ap_mrb_syslogger(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value argc, *argv;
    char *i_pri,*msg;
    int i;
    int pri = INVALID_PRIORITY;

    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
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
    syslog(pri, msg);
    closelog();

    return str;

}
