#ifndef AP_MRB_FILTER_H
#define AP_MRB_FILTER_H

#include "ap_mrb_request.h"

typedef struct {
    ap_filter_t *f;
    apr_bucket_brigade *bb;
} ap_mrb_filter_rec;

int ap_mrb_push_filter(ap_filter_t *f, apr_bucket_brigade *bb);
ap_mrb_filter_rec *ap_mrb_get_filter();

//void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core);

#endif
