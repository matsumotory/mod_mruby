#ifndef AP_MRB_FILTER_H
#define AP_MRB_FILTER_H

#include "ap_mrb_request.h"

typedef struct {
  ap_filter_t *f;
  apr_bucket_brigade *bb;
  apr_finfo_t *finfo;
  apr_bucket_file *file_bucket;
} ap_mrb_filter_rec;

void ap_mrb_set_filter_rec(ap_filter_t *f, apr_bucket_brigade *bb,
                           apr_pool_t *pool);
ap_mrb_filter_rec *ap_mrb_get_filter_rec(apr_pool_t *pool);

// void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core);

#endif
