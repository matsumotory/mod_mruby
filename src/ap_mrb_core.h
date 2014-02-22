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

#include "http_protocol.h"
#include "mruby/string.h"
#include "mruby/variable.h"

#define INVALID_PRIORITY  -1

//int mod_mruby_return_code;

typedef struct _code {
  char *c_name;
  int c_val;
} CODE;

int ap_mrb_get_status_code();
int ap_mrb_set_status_code(int val);
void ap_mrb_raise_error(mrb_state *mrb, mrb_value obj, mod_mruby_code_t *code);

#endif
