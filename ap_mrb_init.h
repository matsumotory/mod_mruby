#ifndef AP_MRB_INIT_H
#define AP_MRB_INIT_H

struct RClass *class;
struct RClass *class_request;
struct RClass *class_server;

int ap_mruby_class_init(mrb_state *mrb);

#endif
