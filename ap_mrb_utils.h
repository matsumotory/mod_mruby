#include <unistd.h>
#include <string.h>
#include <mruby/string.h>
#include <sys/syslog.h>

#define INVALID_PRIORITY    -1

mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_syslogger(mrb_state *mrb, mrb_value str);


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
    while (prioritynames[i].c_name != NULL) {
        if (strcmp(prioritynames[i].c_name, i_pri) == 0) {
            pri = prioritynames[i].c_val;
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
