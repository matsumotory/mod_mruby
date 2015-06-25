/*
// ap_mrb_server.c - to provide server api
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_server.h"
//#include "json.h"

//static struct mrb_data_type server_rec_type = {
//  "server_rec", 0,
//};

// char write
static mrb_value ap_mrb_set_server_error_fname(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->server->error_fname = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

// char read
static mrb_value ap_mrb_get_server_error_fname(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->error_fname);
}

static mrb_value ap_mrb_get_server_document_root(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  char *val = apr_pstrdup(r->pool, ap_document_root(r));
  return mrb_str_new(mrb, val, strlen(val));
}

static mrb_value ap_mrb_get_server_hostname(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->server_hostname);
}

static mrb_value ap_mrb_get_server_path(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->path);
}

static mrb_value ap_mrb_get_server_admin(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->server_admin);
}

static mrb_value ap_mrb_get_server_scheme(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->server_scheme);
}

static mrb_value ap_mrb_get_server_defn_name(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return ap_mrb_str_to_value(mrb, r->pool, r->server->defn_name);
}


// int write
static mrb_value ap_mrb_set_server_loglevel(mrb_state *mrb, mrb_value str)
{
  mrb_int val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "i", &val);
#ifdef __APACHE24__
  r->server->log.level = (int)val;
#else
  r->server->loglevel = (int)val;
#endif
  return str;
}

// int read
static mrb_value ap_mrb_get_server_loglevel(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
#ifdef __APACHE24__
  return mrb_fixnum_value(r->server->log.level);
#else
  return mrb_fixnum_value(r->server->loglevel);
#endif
}

static mrb_value ap_mrb_get_server_is_virtual(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->is_virtual);
}

static mrb_value ap_mrb_get_server_keep_alive_max(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->keep_alive_max);
}

static mrb_value ap_mrb_get_server_keep_alive(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->keep_alive);
}

static mrb_value ap_mrb_get_server_pathlen(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->pathlen);
}

static mrb_value ap_mrb_get_server_limit_req_line(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->limit_req_line);
}

static mrb_value ap_mrb_get_server_limit_req_fieldsize(mrb_state *mrb,
    mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->limit_req_fieldsize);
}

static mrb_value ap_mrb_get_server_limit_req_fields(mrb_state *mrb,
    mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->limit_req_fields);
}

static mrb_value ap_mrb_get_server_timeout(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->timeout);
}

static mrb_value ap_mrb_get_server_keep_alive_timeout(mrb_state *mrb,
    mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->keep_alive_timeout);
}

static mrb_value ap_mrb_get_server_port(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->port);
}

static mrb_value ap_mrb_get_server_defn_line_number(mrb_state *mrb,
    mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value(r->server->defn_line_number);
}

void ap_mruby_server_init(mrb_state *mrb, struct RClass *class_core)
{
  struct RClass *class_server;

  class_server = mrb_define_class_under(mrb, class_core, "Server", mrb->object_class);
  mrb_define_method(mrb, class_server, "error_fname=", ap_mrb_set_server_error_fname, MRB_ARGS_ANY());
  mrb_define_method(mrb, class_server, "error_fname", ap_mrb_get_server_error_fname, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "document_root", ap_mrb_get_server_document_root, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "loglevel=", ap_mrb_set_server_loglevel, MRB_ARGS_ANY());
  mrb_define_method(mrb, class_server, "loglevel", ap_mrb_get_server_loglevel, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "hostname", ap_mrb_get_server_hostname, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "path", ap_mrb_get_server_path, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "admin", ap_mrb_get_server_admin, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "redirect_server_scheme", ap_mrb_get_server_scheme, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "defn_name", ap_mrb_get_server_defn_name, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "is_virtual", ap_mrb_get_server_is_virtual, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "keep_alive_max", ap_mrb_get_server_keep_alive_max, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "keep_alive", ap_mrb_get_server_keep_alive, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "pathlen", ap_mrb_get_server_pathlen, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "limit_req_line", ap_mrb_get_server_limit_req_line, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "limit_req_fieldsize", ap_mrb_get_server_limit_req_fieldsize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "limit_req_fields", ap_mrb_get_server_limit_req_fields, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "timeout", ap_mrb_get_server_timeout, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "keep_alive_timeout", ap_mrb_get_server_keep_alive_timeout, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "redirect_server_port", ap_mrb_get_server_port, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_server, "defn_line_number", ap_mrb_get_server_defn_line_number, MRB_ARGS_NONE());
}
