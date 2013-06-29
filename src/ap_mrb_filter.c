/*
// ap_mrb_filter.c - to provide filter class
//
// See Copyright Notice in mod_mruby.h
*/

#include "ap_mrb_filter.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "mruby/variable.h"

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

mrb_value ap_mrb_filter_init(mrb_state *mrb, mrb_value self)
{
    request_rec *r = ap_mrb_get_request();
    return self;
}

static void mrb_apr_bucket_brigade_free(mrb_state *mrb, void *p)
{
}

static const struct mrb_data_type mrb_apr_bucket_brigade_type = {
    "apr_bucket_brigade", mrb_apr_bucket_brigade_free,
};

static void mrb_apr_bucket_free(mrb_state *mrb, void *p)
{
}

static const struct mrb_data_type mrb_apr_bucket_type = {
    "apr_bucket", mrb_apr_bucket_free,
};

static mrb_value ap_mrb_filter_brigade_first(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    const apr_bucket *bkt = APR_BRIGADE_FIRST(ff->bb);
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, &mrb_apr_bucket_type, (void*) bkt));
}


static mrb_value ap_mrb_filter_puts(mrb_state *mrb, mrb_value self)
{
    char *str;
    apr_status_t rv;
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    mrb_get_args(mrb, "z", &str);
    rv = apr_brigade_puts(ff->bb, NULL, NULL, str);
    return mrb_fixnum_value((int)rv);
}

static mrb_value ap_mrb_filter_insert_tail(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    mrb_value bkt_o;
    apr_bucket *b;
    mrb_get_args(mrb, "o", &bkt_o);
    b = (apr_bucket *)mrb_check_datatype(mrb, bkt_o, &mrb_apr_bucket_type);
    APR_BRIGADE_INSERT_TAIL(ff->bb, b);
    return self;
}

static mrb_value ap_mrb_filter_insert_eos(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    APR_BRIGADE_INSERT_TAIL(ff->bb, apr_bucket_eos_create(ff->f->c->bucket_alloc));
    return self;
}

static mrb_value ap_mrb_filter_brigade_destroy(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    apr_status_t rv = apr_brigade_destroy(ff->bb);
    return mrb_fixnum_value(rv);
}

static mrb_value ap_mrb_filter_brigade_cleanup(mrb_state *mrb, mrb_value self)
{
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    apr_status_t rv = apr_brigade_cleanup(ff->bb);
    return mrb_fixnum_value(rv);
}

static mrb_value ap_mrb_filter_brigade_pflatten(mrb_state *mrb, mrb_value self)
{
    char *data;
    apr_size_t len;
    apr_status_t rv;
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    rv = apr_brigade_pflatten(ff->bb, &data, &len, ff->f->r->pool);
    if (rv) 
        mrb_raise(mrb, E_RUNTIME_ERROR, "apr_brigade_pflatten failed");
    return mrb_str_new(mrb, data, len);
}

static mrb_value ap_mrb_filter_brigade_length(mrb_state *mrb, mrb_value self)
{
    apr_off_t len;
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    apr_brigade_length(ff->bb, 1, &len);
    return mrb_fixnum_value(len);
}

static mrb_value ap_mrb_filter_brigade_empty(mrb_state *mrb, mrb_value self)
{
    apr_status_t rv;
    ap_mrb_filter_rec *ff = ap_mrb_get_filter();
    rv = APR_BRIGADE_EMPTY(ff->bb);
    return (rv) ? mrb_true_value()  : mrb_false_value();
}

static mrb_value ap_mrb_filter_brigade_is_eos(mrb_state *mrb, mrb_value self)
{
    apr_status_t rv;
    apr_bucket *b;
    //mrb_value bkt_o;
    //mrb_get_args(mrb, "o", &bkt_o);
    b = (apr_bucket *)mrb_check_datatype(mrb, self, &mrb_apr_bucket_type);
    rv = APR_BUCKET_IS_EOS(b);
    return (rv) ? mrb_true_value()  : mrb_false_value();
}

static mrb_value ap_mrb_filter_bucket_read(mrb_state *mrb, mrb_value self)
{
    apr_size_t len;
    const char* data;
    apr_status_t rv;
    apr_bucket *b;
    mrb_value bkt_o;
    mrb_get_args(mrb, "o", &bkt_o);
    b = (apr_bucket *)mrb_check_datatype(mrb, bkt_o, &mrb_apr_bucket_type);
    apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
    return mrb_str_new(mrb, data, len);
}

void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_filter;

    mrb_define_method(mrb, mrb->kernel_module, "is_eos?", ap_mrb_filter_brigade_is_eos, ARGS_NONE());

    class_filter = mrb_define_class_under(mrb, class_core, "Filter", mrb->object_class);
    mrb_define_method(mrb, class_filter, "initialize", ap_mrb_filter_init, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "puts", ap_mrb_filter_puts, ARGS_REQ(1));
    mrb_define_method(mrb, class_filter, "insert_tail", ap_mrb_filter_insert_tail, ARGS_REQ(1));
    mrb_define_method(mrb, class_filter, "insert_eos", ap_mrb_filter_insert_eos, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "destroy", ap_mrb_filter_brigade_destroy, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "cleanup", ap_mrb_filter_brigade_cleanup, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "flatten", ap_mrb_filter_brigade_pflatten, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "length", ap_mrb_filter_brigade_length, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "empty?", ap_mrb_filter_brigade_empty, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "first_bucket", ap_mrb_filter_brigade_first, ARGS_NONE());
    mrb_define_method(mrb, class_filter, "bucket_read", ap_mrb_filter_bucket_read, ARGS_REQ(1));
    
}
