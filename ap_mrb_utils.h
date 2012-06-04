#ifndef AP_MRB_UTILS_H
#define AP_MRB_UTILS_H

#ifndef _WIN32
#include <unistd.h>
#include <string.h>
#include <sys/syslog.h>
#else
#include <string.h>
#endif
#include "mruby.h"


#define INVALID_PRIORITY    -1

int mod_mruby_return_code;

typedef struct _code {
    char    *c_name;
    int c_val;
} CODE;

int ap_mrb_get_status_code();
int ap_mrb_set_status_code(int val);
mrb_value ap_mrb_return(mrb_state *mrb, mrb_value self);
mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_syslogger(mrb_state *mrb, mrb_value str);

#endif
