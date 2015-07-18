/*
// ap_mrb_filter.c - to provide filter class
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_filter.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "mruby/variable.h"
#include "http_protocol.h"

void ap_mrb_set_filter_rec(ap_filter_t *f, apr_bucket_brigade *bb,
                           apr_pool_t *pool)
{
  ap_mrb_filter_rec *fr =
      (ap_mrb_filter_rec *)apr_pcalloc(f->r->pool, sizeof(ap_mrb_filter_rec));
  apr_finfo_t finfo;
  apr_file_t *fd;
  apr_bucket *b = APR_BRIGADE_FIRST(bb);

  if (APR_BUCKET_IS_FILE(b)) {
    fr->file_bucket = (apr_bucket_file *)(b->data);
    fd = fr->file_bucket->fd;
    if (apr_file_info_get(&finfo, APR_FINFO_NORM, fd) == APR_SUCCESS) {
      fr->finfo = &finfo;
    }
  } else {
    fr->file_bucket = NULL;
    fr->finfo = NULL;
  }
  fr->f = f;
  fr->bb = bb;

  apr_pool_userdata_set(fr, "mod_mruby_filter_rec", NULL, pool);
}

ap_mrb_filter_rec *ap_mrb_get_filter_rec(apr_pool_t *pool)
{
  ap_mrb_filter_rec *fr = NULL;
  if (apr_pool_userdata_get((void **)&fr, "mod_mruby_filter_rec", pool) ==
      APR_SUCCESS) {
    if (fr == NULL) {
      ap_log_error(
          APLOG_MARK, APLOG_ERR, 0, NULL,
          "%s ERROR %s: apr_pool_userdata_get mod_mruby_filter_rec success, "
          "but fr is NULL",
          MODULE_NAME, __func__);
      return NULL;
    }
    return fr;
  }
  ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL,
               "%s ERROR %s: apr_pool_userdata_get mod_mruby_filter_rec faled",
               MODULE_NAME, __func__);
  return NULL;
}

mrb_value ap_mrb_filter_init(mrb_state *mrb, mrb_value self)
{
  return self;
}

static void mrb_apr_bucket_free(mrb_state *mrb, void *p)
{
}

static const struct mrb_data_type mrb_apr_bucket_type = {
    "apr_bucket", mrb_apr_bucket_free,
};

static mrb_value ap_mrb_filter_brigade_first(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  const apr_bucket *bkt = APR_BRIGADE_FIRST(ff->bb);
  return mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class,
                                        &mrb_apr_bucket_type, (void *)bkt));
}

static mrb_value ap_mrb_filter_puts(mrb_state *mrb, mrb_value self)
{
  char *str;
  apr_status_t rv;
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  mrb_get_args(mrb, "z", &str);
  rv = apr_brigade_puts(ff->bb, NULL, NULL, str);
  return mrb_fixnum_value((int)rv);
}

static mrb_value ap_mrb_filter_insert_tail(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  char *s;
  apr_bucket *b;
  mrb_get_args(mrb, "z", &s);
  b = apr_bucket_immortal_create(s, strlen(s), ff->f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ff->bb, b);
  return self;
}

static mrb_value ap_mrb_filter_insert_head(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  char *s;
  apr_bucket *b;
  mrb_get_args(mrb, "z", &s);
  b = apr_bucket_immortal_create(s, strlen(s), ff->f->c->bucket_alloc);
  APR_BRIGADE_INSERT_HEAD(ff->bb, b);
  return self;
}

static mrb_value ap_mrb_filter_insert_eos(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  APR_BRIGADE_INSERT_TAIL(ff->bb,
                          apr_bucket_eos_create(ff->f->c->bucket_alloc));
  return self;
}

static mrb_value ap_mrb_filter_brigade_destroy(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  apr_status_t rv = apr_brigade_destroy(ff->bb);
  return mrb_fixnum_value(rv);
}

static mrb_value ap_mrb_filter_brigade_cleanup(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  apr_status_t rv = apr_brigade_cleanup(ff->bb);
  return mrb_fixnum_value(rv);
}

static mrb_value ap_mrb_filter_brigade_pflatten(mrb_state *mrb, mrb_value self)
{
  char *data;
  apr_size_t len;
  apr_status_t rv;
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  rv = apr_brigade_pflatten(ff->bb, &data, &len, ff->f->r->pool);
  if (rv)
    mrb_raise(mrb, E_RUNTIME_ERROR, "apr_brigade_pflatten failed");
  return mrb_str_new_cstr(mrb, data);
}

static mrb_value ap_mrb_filter_brigade_length(mrb_state *mrb, mrb_value self)
{
  apr_off_t len;
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  apr_brigade_length(ff->bb, 1, &len);
  return mrb_fixnum_value(len);
}

static mrb_value ap_mrb_filter_brigade_empty(mrb_state *mrb, mrb_value self)
{
  apr_status_t rv;
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  rv = APR_BRIGADE_EMPTY(ff->bb);
  return (rv) ? mrb_true_value() : mrb_false_value();
}

// static mrb_value ap_mrb_filter_brigade_is_eos(mrb_state *mrb, mrb_value self)
//{
//  apr_status_t rv;
//  apr_bucket *b;
//  //mrb_value bkt_o;
//  //mrb_get_args(mrb, "o", &bkt_o);
//  b = (apr_bucket *)mrb_check_datatype(mrb, self, &mrb_apr_bucket_type);
//  rv = APR_BUCKET_IS_EOS(b);
//  return (rv) ? mrb_true_value()  : mrb_false_value();
//}
//
// static mrb_value ap_mrb_filter_bucket_read(mrb_state *mrb, mrb_value self)
//{
//  apr_size_t len;
//  const char* data;
//  apr_status_t rv;
//  apr_bucket *b;
//  mrb_value bkt_o;
//  mrb_get_args(mrb, "o", &bkt_o);
//  b = (apr_bucket *)mrb_check_datatype(mrb, bkt_o, &mrb_apr_bucket_type);
//  apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
//  return mrb_str_new(mrb, data, len);
//}

static mrb_value ap_mrb_filter_uid(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  apr_file_t *fd = ff->file_bucket->fd;
  apr_finfo_t finfo;
  if (apr_file_info_get(&finfo, APR_FINFO_USER, fd) != APR_SUCCESS) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(finfo.user);
}

static mrb_value ap_mrb_filter_gid(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  apr_file_t *fd = ff->file_bucket->fd;
  apr_finfo_t finfo;
  if (apr_file_info_get(&finfo, APR_FINFO_GROUP, fd) != APR_SUCCESS) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(finfo.group);
}

static mrb_value ap_mrb_filter_error_create(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  ap_mrb_filter_rec *ff = ap_mrb_get_filter_rec(r->pool);
  ap_filter_t *f = ff->f;
  apr_bucket *b;
  mrb_int status;

  mrb_get_args(mrb, "i", &status);

  apr_brigade_cleanup(ff->bb);
  b = ap_bucket_error_create(status, NULL, f->r->pool, f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ff->bb, b);
  b = apr_bucket_eos_create(f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ff->bb, b);

  return mrb_fixnum_value(status);
}

void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core)
{
  struct RClass *class_filter;

  class_filter =
      mrb_define_class_under(mrb, class_core, "Filter", mrb->object_class);
  mrb_define_method(mrb, class_filter, "initialize", ap_mrb_filter_init,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "puts", ap_mrb_filter_puts,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_filter, "insert_tail", ap_mrb_filter_insert_tail,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_filter, "insert_head", ap_mrb_filter_insert_head,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_filter, "insert_eos", ap_mrb_filter_insert_eos,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "destroy", ap_mrb_filter_brigade_destroy,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "cleanup", ap_mrb_filter_brigade_cleanup,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "flatten",
                    ap_mrb_filter_brigade_pflatten, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "length", ap_mrb_filter_brigade_length,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "empty?", ap_mrb_filter_brigade_empty,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "first_bucket",
                    ap_mrb_filter_brigade_first, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "uid", ap_mrb_filter_uid,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "gid", ap_mrb_filter_gid,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_filter, "error_create",
                    ap_mrb_filter_error_create, MRB_ARGS_REQ(1));
  // mrb_define_method(mrb, class_filter, "bucket_read",
  // ap_mrb_filter_bucket_read, MRB_ARGS_REQ(1));
}
