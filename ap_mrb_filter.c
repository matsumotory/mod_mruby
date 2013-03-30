/*
// ap_mrb_filter.c - to provide filter class
//
// See Copyright Notice in mod_mruby.h
*/

#include "ap_mrb_filter.h"
#include "mruby/string.h"
#include "mruby/hash.h"

ap_mrb_filter_rec *mrb_filter_state = NULL;

int ap_mrb_push_filter(ap_filter_t *f, apr_bucket_brigade *bb)
{
    mrb_filter_state = (ap_mrb_filter_rec *)apr_pcalloc(f->r->pool, sizeof(ap_mrb_filter_rec *));
    mrb_filter_state->f = f;
    mrb_filter_state->bb = bb;
    return OK;
}

ap_mrb_filter_rec *ap_mrb_get_filter()
{
    return mrb_filter_state;
}

static mrb_value ap_mrb_filter_init(mrb_state *mrb, mrb_value self)
{
    request_rec *r = ap_mrb_get_request();
    return self;
}

static mrb_value ap_mrb_filter_insert_tail(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    char *str;
    mrb_get_args(mrb, "z", &str);
    APR_BRIGADE_INSERT_TAIL(ff->bb, apr_bucket_immortal_create(str, strlen(str), ff->f->c->bucket_alloc));
    return self;
}

static mrb_value ap_mrb_filter_insert_eos(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    APR_BRIGADE_INSERT_TAIL(ff->bb, apr_bucket_eos_create(ff->f->c->bucket_alloc));
    return self;
}

static mrb_value ap_mrb_filter_cleanup(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    apr_brigade_cleanup(ff->bb);
    return self;
}

static mrb_value ap_mrb_filter_pflatten(mrb_state *mrb, mrb_value self)
{
    char *buf;
    apr_size_t len;
    apr_status_t err;
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    err = apr_brigade_pflatten(ff->bb, &buf, &len, ff->f->r->pool);
    if (err) 
        mrb_raise(mrb, E_RUNTIME_ERROR, "apr_brigade_pflatten failed");
    return mrb_str_new(mrb, buf, len);
}

void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_filter;

    class_filter = mrb_define_class_under(mrb, class_core, "Filter", mrb->object_class);
    mrb_define_method(mrb, class_filter, "initialize", ap_mrb_filter_init, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "insert_tail", ap_mrb_filter_insert_tail, ARGS_REQ(1));
    mrb_define_method(mrb, class_filter, "insert_eos", ap_mrb_filter_insert_eos, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "cleanup", ap_mrb_filter_cleanup, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "flatten", ap_mrb_filter_pflatten, ARGS_NONE());
}
