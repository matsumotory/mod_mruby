#ifndef AP_MRB_UTILS_H
#define AP_MRB_UTILS_H

#include <unistd.h>
#include <string.h>
#include <sys/syslog.h>
#include "mruby.h"


#define INVALID_PRIORITY    -1

typedef struct _code {
    char    *c_name;
    int c_val;
} CODE;


mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_syslogger(mrb_state *mrb, mrb_value str);

#endif
