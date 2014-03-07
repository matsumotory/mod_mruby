/*
// ap_mrb_request.c - to provide request api
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_init.h"
#include "ap_mrb_request.h"
#include "mruby/hash.h"

#define CORE_PRIVATE
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
//#include "apr_table.h"
//#include "json.h"


/*
static struct mrb_data_type request_rec_type = {
  "request_rec", 0,
};
*/


request_rec *mrb_request_rec_state = NULL;

int ap_mrb_push_request(request_rec *r)
{
  mrb_request_rec_state = (request_rec *)apr_pcalloc(r->pool, sizeof (*mrb_request_rec_state));
  mrb_request_rec_state = r;
  return OK;
}

request_rec *ap_mrb_get_request()
{
  return mrb_request_rec_state;
}

mrb_value ap_mrb_str_to_value(mrb_state *mrb, apr_pool_t *p, const char *str)
{
  const char *val;

  if (str == NULL) {
    return mrb_nil_value();
  }
  val = apr_pstrdup(p, str);
  return mrb_str_new(mrb, val, strlen(val));
}

/*
mrb_value ap_mrb_init_request(mrb_state *mrb, mrb_value str)
{
  str = mrb_class_new_instance(mrb, 0, NULL, class_request);
  mrb_iv_set(mrb
    , str
    , mrb_intern(mrb, "request_rec")
    , mrb_obj_value(Data_Wrap_Struct(mrb
      , mrb->object_class
      , &request_rec_type
      , ap_mrb_get_request())
    )
  );

  ap_log_error(APLOG_MARK
    , APLOG_WARNING
    , 0
    , NULL
    , "%s ERROR %s: Initialied."
    , MODULE_NAME
    , __func__
  );

  return str;
}
*/

/*
mrb_value ap_mrb_get_request_rec_json(mrb_state *mrb, mrb_value str)
{
  char *val;
  request_rec *r = ap_mrb_get_request();
  json_object *my_object;

  my_object = json_object_new_object();
  json_object_object_add(my_object, "filename", json_object_new_string(ap_mrb_string_check(r->pool, r->filename)));
  json_object_object_add(my_object, "uri", json_object_new_string(ap_mrb_string_check(r->pool, r->uri)));
  json_object_object_add(my_object, "user", json_object_new_string(ap_mrb_string_check(r->pool, r->user)));
  json_object_object_add(my_object, "content_type", json_object_new_string(ap_mrb_string_check(r->pool, r->content_type)));
  json_object_object_add(my_object, "protocol", json_object_new_string(ap_mrb_string_check(r->pool, r->protocol)));
  json_object_object_add(my_object, "vlist_validator", json_object_new_string(ap_mrb_string_check(r->pool, r->vlist_validator)));
  json_object_object_add(my_object, "ap_auth_type", json_object_new_string(ap_mrb_string_check(r->pool, r->ap_auth_type)));
  json_object_object_add(my_object, "unparsed_uri", json_object_new_string(ap_mrb_string_check(r->pool, r->unparsed_uri)));
  json_object_object_add(my_object, "canonical_filename", json_object_new_string(ap_mrb_string_check(r->pool, r->canonical_filename)));
  json_object_object_add(my_object, "path_info", json_object_new_string(ap_mrb_string_check(r->pool, r->path_info)));
  json_object_object_add(my_object, "hostname", json_object_new_string(ap_mrb_string_check(r->pool, r->hostname)));

  val = (char *)json_object_to_json_string(my_object);

  if (val == NULL)
    val = apr_pstrdup(r->pool, "(null)");
  
  return mrb_str_new(mrb, val, strlen(val));

}
*/

static mrb_value ap_mrb_replace_stderr_log(mrb_state *mrb, mrb_value self)
{
  const char *file;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "z", &file);
  return mrb_fixnum_value(ap_replace_stderr_log(r->pool, file));
}

static mrb_value ap_mrb_get_request_body(mrb_state *mrb, mrb_value str)
{
  char *val;
  int len;
  request_rec *r = ap_mrb_get_request();
  if (r->method_number == M_POST) {
    ap_setup_client_block(r, REQUEST_CHUNKED_ERROR);
    len = r->remaining;
    val = apr_pcalloc(r->pool, len);
    ap_should_client_block(r);
    ap_get_client_block(r, val, len);
    return mrb_str_new(mrb, val, len);
  }
  return mrb_nil_value();
}

#define AP_MRB_GET_REQUEST_VALUE(value) \
static mrb_value ap_mrb_get_request_##value(mrb_state *mrb, mrb_value self)             \
{                                                             \
  request_rec *r = ap_mrb_get_request(); \
  return ap_mrb_str_to_value(mrb, r->pool, r->value); \
}

AP_MRB_GET_REQUEST_VALUE(the_request)
AP_MRB_GET_REQUEST_VALUE(protocol)
AP_MRB_GET_REQUEST_VALUE(vlist_validator)
AP_MRB_GET_REQUEST_VALUE(user)
AP_MRB_GET_REQUEST_VALUE(ap_auth_type)
AP_MRB_GET_REQUEST_VALUE(unparsed_uri)
AP_MRB_GET_REQUEST_VALUE(uri)
AP_MRB_GET_REQUEST_VALUE(filename)
AP_MRB_GET_REQUEST_VALUE(canonical_filename)
AP_MRB_GET_REQUEST_VALUE(path_info)
AP_MRB_GET_REQUEST_VALUE(args)
AP_MRB_GET_REQUEST_VALUE(hostname)

static mrb_value ap_mrb_get_request_document_root(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  char *val = apr_pstrdup(r->pool, ap_document_root(r));
  return mrb_str_new(mrb, val, strlen(val));
}

AP_MRB_GET_REQUEST_VALUE(status_line)
AP_MRB_GET_REQUEST_VALUE(method)
AP_MRB_GET_REQUEST_VALUE(range)
AP_MRB_GET_REQUEST_VALUE(content_type)

static mrb_value ap_mrb_get_request_content_length(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  mrb_int val = r->clength;
  return mrb_fixnum_value(val);
}

AP_MRB_GET_REQUEST_VALUE(handler)
AP_MRB_GET_REQUEST_VALUE(content_encoding)


static mrb_value ap_mrb_set_request_the_request(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->the_request = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_protocol(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->protocol = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_vlist_validator(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->vlist_validator = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_user(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->user = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_ap_auth_type(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->ap_auth_type = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_unparsed_uri(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->unparsed_uri = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_uri(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->uri = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->filename = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_canonical_filename(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->canonical_filename = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_path_info(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->path_info = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_args(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->args = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_hostname(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->hostname = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_document_root(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
#ifdef __APACHE24__
  ap_set_document_root(r, (const char *)mrb_str_to_cstr(mrb, val));
#else
  core_server_config *conf = (core_server_config *)ap_get_module_config(r->server->module_config, &core_module);
  conf->ap_document_root = (const char *)mrb_str_to_cstr(mrb, val);
#endif
  return val;
}

static mrb_value ap_mrb_set_request_status_line(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->status_line = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_status(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->status = mrb_fixnum(val);
  return val;
}

static mrb_value ap_mrb_set_request_method(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->method = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_range(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->range = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_content_type(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->content_type = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_content_length(mrb_state *mrb, mrb_value str)
{
  mrb_int val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "i", &val);
  ap_set_content_length(r, (apr_off_t)val);
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_set_request_handler(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->handler = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_content_encoding(mrb_state *mrb, mrb_value str)
{
  mrb_value val;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "o", &val);
  r->content_encoding = apr_pstrdup(r->pool, mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_set_request_notes(mrb_state *mrb, mrb_value str)
{
  mrb_value key, val;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "oo", &key, &val);
  apr_table_set(r->notes, mrb_str_to_cstr(mrb, key), mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_get_request_notes(mrb_state *mrb, mrb_value str)
{
  mrb_value key;
  request_rec *r = ap_mrb_get_request();
  const char *val;

  mrb_get_args(mrb, "o", &key);
  val = apr_table_get(r->notes, mrb_str_to_cstr(mrb, key));
  if (val == NULL)
    return mrb_nil_value();
  return mrb_str_new(mrb, val, strlen(val));
}

static mrb_value ap_mrb_set_request_headers_in(mrb_state *mrb, mrb_value str)
{
  mrb_value key, val;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "oo", &key, &val);
  apr_table_set(r->headers_in, mrb_str_to_cstr(mrb, key), mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_get_request_headers_in(mrb_state *mrb, mrb_value str)
{
  mrb_value key;
  request_rec *r = ap_mrb_get_request();
  const char *val;

  mrb_get_args(mrb, "o", &key);
  val = apr_table_get(r->headers_in, mrb_str_to_cstr(mrb, key));
  if (val == NULL)
    return mrb_nil_value();
  return mrb_str_new(mrb, val, strlen(val));
}

static mrb_value ap_mrb_get_request_headers_in_hash(mrb_state *mrb, mrb_value str)
{
  int i;
  mrb_value hash = mrb_hash_new(mrb);
  request_rec *r = ap_mrb_get_request();
  const apr_array_header_t *arr = apr_table_elts(r->headers_in);
  apr_table_entry_t *elts = (apr_table_entry_t *)arr->elts;
  for (i = 0; i < arr->nelts; i++) {
    mrb_hash_set(mrb
      , hash
      , mrb_str_new(mrb, elts[i].key, strlen(elts[i].key))
      , mrb_str_new(mrb, elts[i].val, strlen(elts[i].val))
    );
  }
  return hash;
}

static mrb_value ap_mrb_set_request_headers_out(mrb_state *mrb, mrb_value str)
{
  mrb_value key, val;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "oo", &key, &val);
  apr_table_set(r->headers_out, mrb_str_to_cstr(mrb, key), mrb_str_to_cstr(mrb, val));
  return val;
}

static mrb_value ap_mrb_get_request_headers_out(mrb_state *mrb, mrb_value str)
{
  mrb_value key;
  const char *val;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "o", &key);
  val = apr_table_get(r->headers_out, mrb_str_to_cstr(mrb, key));
  return mrb_str_new(mrb, val, strlen(val));
}

static mrb_value ap_mrb_get_request_headers_out_hash(mrb_state *mrb, mrb_value str)
{
  int i;
  mrb_value hash = mrb_hash_new(mrb);
  request_rec *r = ap_mrb_get_request();
  const apr_array_header_t *arr = apr_table_elts(r->headers_out);
  apr_table_entry_t *elts = (apr_table_entry_t *)arr->elts;
  for (i = 0; i < arr->nelts; i++) {
    mrb_hash_set(mrb
      , hash
      , mrb_str_new(mrb, elts[i].key, strlen(elts[i].key))
      , mrb_str_new(mrb, elts[i].val, strlen(elts[i].val))
    );
  }
  return hash;
}

//
// Apache::Finfo (r->finfo)
//

//static mrb_value ap_mrb_get_request_finfo(mrb_state *mrb, mrb_value str)
//{
//// TODO
////struct apr_finfo_t {
////  /* Allocates memory and closes lingering handles in the specified pool */
////  apr_pool_t *pool;
////  /** The bitmask describing valid fields of this apr_finfo_t structure 
////   *  including all available 'wanted' fields and potentially more */
////  apr_int32_t valid;
////  /** The access permissions of the file.  Mimics Unix access rights. */
////  apr_fileperms_t protection;
////  /** The type of file.  One of APR_REG, APR_DIR, APR_CHR, APR_BLK, APR_PIPE, 
////   * APR_LNK or APR_SOCK.  If the type is undetermined, the value is APR_NOFILE.
////   * If the type cannot be determined, the value is APR_UNKFILE.
////   */
////  apr_filetype_e filetype;
////  /** The user id that owns the file */
////  apr_uid_t user;
////  /** The group id that owns the file */
////  apr_gid_t group;
////  /** The inode of the file. */
////  apr_ino_t inode;
////  /** The id of the device the file is on. */
////  apr_dev_t device;
////  /** The number of hard links to the file. */
////  apr_int32_t nlink;
////  /** The size of the file */
////  apr_off_t size;
////  /** The storage size consumed by the file */
////  apr_off_t csize;
////  /** The time the file was last accessed */
////  apr_time_t atime;
////  /** The time the file was last modified */
////  apr_time_t mtime;
////  /** The time the file was created, or the inode was last changed */
////  apr_time_t ctime;
////  /** The pathname of the file (possibly unrooted) */
////  const char *fname;
////  /** The file's name (no path) in filesystem case */
////  const char *name;
////  /** The file's handle, if accessed (can be submitted to apr_duphandle) */
////  struct apr_file_t *filehand;
////};
//  mrb_value key;
//  request_rec *r = ap_mrb_get_request();
//  const char *val;
//
//  mrb_get_args(mrb, "o", &key);
//  val = apr_table_get(r->headers_in, mrb_str_to_cstr(mrb, key));
//  if (val == NULL)
//    return mrb_nil_value();
//  return mrb_str_new(mrb, val, strlen(val));
//}

static mrb_value ap_mrb_get_request_finfo_protection(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.protection);
}

static mrb_value ap_mrb_get_request_finfo_filetype(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.filetype);
}

static mrb_value ap_mrb_get_request_finfo_inode(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.inode);
}

static mrb_value ap_mrb_get_request_finfo_device(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.device);
}

static mrb_value ap_mrb_get_request_finfo_nlink(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.nlink);
}

static mrb_value ap_mrb_get_request_finfo_group(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.group);
}

static mrb_value ap_mrb_get_request_finfo_user(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_fixnum_value((mrb_int)r->finfo.user);
}

static mrb_value ap_mrb_get_request_finfo_size(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_float_value(mrb, (mrb_float)r->finfo.size);
}

static mrb_value ap_mrb_get_request_finfo_csize(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_float_value(mrb, (mrb_float)r->finfo.csize);
}

static mrb_value ap_mrb_get_request_finfo_atime(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_float_value(mrb, (mrb_float)r->finfo.atime);
}

static mrb_value ap_mrb_get_request_finfo_ctime(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_float_value(mrb, (mrb_float)r->finfo.ctime);
}

static mrb_value ap_mrb_get_request_finfo_mtime(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  return mrb_float_value(mrb, (mrb_float)r->finfo.mtime);
}

static mrb_value ap_mrb_get_request_assbackwards(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->assbackwards;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_set_request_proxyreq(mrb_state *mrb, mrb_value str)
{

  mrb_int ret;
  request_rec *r = ap_mrb_get_request();
  mrb_get_args(mrb, "i", &ret);
  r->proxyreq = (int)ret;

  return str;
}

static mrb_value ap_mrb_get_request_proxyreq(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->proxyreq;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_header_only(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->header_only;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_proto_num(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->proto_num;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_status(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->status;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_method_number(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->method_number;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_chunked(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->chunked;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_read_body(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->read_body;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_read_chunked(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->read_chunked;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_used_path_info(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->used_path_info;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_eos_sent(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->eos_sent;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_no_cache(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->no_cache;
  return mrb_fixnum_value(val);
}

static mrb_value ap_mrb_get_request_no_local_copy(mrb_state *mrb, mrb_value str)
{
  request_rec *r = ap_mrb_get_request();
  int val = r->no_local_copy;
  return mrb_fixnum_value(val);
}


//static mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str)
//{   
//
//  struct RProc *b;
//  mrb_value argc, *argv;
//  char *member, *value;
//  request_rec *r = ap_mrb_get_request();
//
//  mrb_get_args(mrb, "b*", &b, &argv, &argc);
//  if (mrb_fixnum(argc) != 2) {
//    ap_log_error(APLOG_MARK
//      , APLOG_WARNING
//      , 0
//      , NULL
//      , "%s ERROR %s: argument is not 2"
//      , MODULE_NAME
//      , __func__
//    );
//    return str;
//  }
//
//  member = mrb_str_to_cstr(mrb, argv[0]);
//  value  = mrb_str_to_cstr(mrb, argv[1]);
//
//  if (strcmp(member, "filename") == 0)
//    r->filename = apr_pstrdup(r->pool, value);
//  else if (strcmp(member, "uri") == 0)
//    r->uri = apr_pstrdup(r->pool, value);
//
//  return str;
//}

static mrb_value ap_mrb_run_handler(mrb_state *mrb, mrb_value self)
{
  request_rec *r = ap_mrb_get_request();
  apr_status_t result = ap_invoke_handler(r);

  if (result != APR_SUCCESS) {
    ap_log_error(APLOG_MARK
      , APLOG_WARNING
      , 0
      , NULL
      , "%s ERROR %s: ap_run_handler failed"
      , MODULE_NAME
      , __func__
    );
    r->connection->aborted = 1;
  }

  return self;
}

static mrb_value ap_mrb_get_class_obj(mrb_state *mrb, mrb_value self, char *obj_id, char *class_name)
{
  mrb_value obj;
  struct RClass *obj_class, *apache_class;

  obj = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, obj_id));
  if (mrb_nil_p(obj)) {
    apache_class = mrb_class_get(mrb, "Apache");
    obj_class = (struct RClass*)mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(apache_class), mrb_intern_cstr(mrb, class_name)));
    obj = mrb_obj_new(mrb, obj_class, 0, NULL);
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, obj_id), obj);
  }
  return obj;
}

static mrb_value ap_mrb_notes_obj(mrb_state *mrb, mrb_value self)
{   
  return ap_mrb_get_class_obj(mrb, self, "notes_obj", "Notes");
}

static mrb_value ap_mrb_headers_in_obj(mrb_state *mrb, mrb_value self)
{   
  return ap_mrb_get_class_obj(mrb, self, "headers_in_obj", "Headers_in");
}

static mrb_value ap_mrb_headers_out_obj(mrb_state *mrb, mrb_value self)
{   
  return ap_mrb_get_class_obj(mrb, self, "headers_out_obj", "Headers_out");
}

static mrb_value ap_mrb_finfo_obj(mrb_state *mrb, mrb_value self)
{   
  return ap_mrb_get_class_obj(mrb, self, "finfo_obj", "Finfo");
}

static mrb_value ap_mrb_request_error_log(mrb_state *mrb, mrb_value self)
{

  mrb_value msg;
  mrb_int pri;
  request_rec *r = ap_mrb_get_request();

  mrb_get_args(mrb, "io", &pri, &msg);
  ap_log_rerror(APLOG_MARK, pri, 0, r, "%s", mrb_str_to_cstr(mrb, msg));

  return msg;
}

void ap_mruby_request_init(mrb_state *mrb, struct RClass *class_core)
{
  struct RClass *class_request;
  struct RClass *class_notes;
  struct RClass *class_headers_in;
  struct RClass *class_headers_out;
  struct RClass *class_finfo;

  class_request = mrb_define_class_under(mrb, class_core, "Request", mrb->object_class);

  mrb_define_method(mrb, class_request, "run_handler", ap_mrb_run_handler, ARGS_NONE());

  //mrb_define_method(mrb, class_request, "Initialize", ap_mrb_init_request, ARGS_NONE());
  //mrb_define_method(mrb, class_request, "request_rec_json", ap_mrb_get_request_rec_json, ARGS_NONE());
  mrb_define_method(mrb, class_request, "body", ap_mrb_get_request_body, ARGS_NONE());

  mrb_define_method(mrb, class_request, "error_log_into", ap_mrb_replace_stderr_log, ARGS_REQ(1));
  mrb_define_method(mrb, class_request, "error_log", ap_mrb_request_error_log, ARGS_REQ(2));
  mrb_define_method(mrb, class_request, "log", ap_mrb_request_error_log, ARGS_REQ(2));
  mrb_define_method(mrb, class_request, "the_request=", ap_mrb_set_request_the_request, ARGS_ANY());
  mrb_define_method(mrb, class_request, "the_request", ap_mrb_get_request_the_request, ARGS_NONE());
  mrb_define_method(mrb, class_request, "protocol=", ap_mrb_set_request_protocol, ARGS_ANY());
  mrb_define_method(mrb, class_request, "protocol", ap_mrb_get_request_protocol, ARGS_NONE());
  mrb_define_method(mrb, class_request, "vlist_validator=", ap_mrb_set_request_vlist_validator, ARGS_ANY());
  mrb_define_method(mrb, class_request, "vlist_validator", ap_mrb_get_request_vlist_validator, ARGS_NONE());
  mrb_define_method(mrb, class_request, "user=", ap_mrb_set_request_user, ARGS_ANY());
  mrb_define_method(mrb, class_request, "user", ap_mrb_get_request_user, ARGS_NONE());
  mrb_define_method(mrb, class_request, "ap_auth_type=", ap_mrb_set_request_ap_auth_type, ARGS_ANY());
  mrb_define_method(mrb, class_request, "ap_auth_type", ap_mrb_get_request_ap_auth_type, ARGS_NONE());
  mrb_define_method(mrb, class_request, "unparsed_uri=", ap_mrb_set_request_unparsed_uri, ARGS_ANY());
  mrb_define_method(mrb, class_request, "unparsed_uri", ap_mrb_get_request_unparsed_uri, ARGS_NONE());
  mrb_define_method(mrb, class_request, "uri=", ap_mrb_set_request_uri, ARGS_ANY());
  mrb_define_method(mrb, class_request, "uri", ap_mrb_get_request_uri, ARGS_NONE());
  mrb_define_method(mrb, class_request, "filename=", ap_mrb_set_request_filename, ARGS_ANY());
  mrb_define_method(mrb, class_request, "filename", ap_mrb_get_request_filename, ARGS_NONE());
  mrb_define_method(mrb, class_request, "canonical_filename=", ap_mrb_set_request_canonical_filename, ARGS_ANY());
  mrb_define_method(mrb, class_request, "canonical_filename", ap_mrb_get_request_canonical_filename, ARGS_NONE());
  mrb_define_method(mrb, class_request, "path_info=", ap_mrb_set_request_path_info, ARGS_ANY());
  mrb_define_method(mrb, class_request, "path_info", ap_mrb_get_request_path_info, ARGS_NONE());
  mrb_define_method(mrb, class_request, "args=", ap_mrb_set_request_args, ARGS_ANY());
  mrb_define_method(mrb, class_request, "args", ap_mrb_get_request_args, ARGS_NONE());
  mrb_define_method(mrb, class_request, "hostname=", ap_mrb_set_request_hostname, ARGS_ANY());
  mrb_define_method(mrb, class_request, "hostname", ap_mrb_get_request_hostname, ARGS_NONE());
  mrb_define_method(mrb, class_request, "document_root=", ap_mrb_set_request_document_root, ARGS_ANY());
  mrb_define_method(mrb, class_request, "document_root", ap_mrb_get_request_document_root, ARGS_NONE());
  mrb_define_method(mrb, class_request, "status_line=", ap_mrb_set_request_status_line, ARGS_ANY());
  mrb_define_method(mrb, class_request, "status_line", ap_mrb_get_request_status_line, ARGS_NONE());
  mrb_define_method(mrb, class_request, "method=", ap_mrb_set_request_method, ARGS_ANY());
  mrb_define_method(mrb, class_request, "method", ap_mrb_get_request_method, ARGS_NONE());
  mrb_define_method(mrb, class_request, "range=", ap_mrb_set_request_range, ARGS_ANY());
  mrb_define_method(mrb, class_request, "range", ap_mrb_get_request_range, ARGS_NONE());
  mrb_define_method(mrb, class_request, "content_type=", ap_mrb_set_request_content_type, ARGS_ANY());
  mrb_define_method(mrb, class_request, "content_type", ap_mrb_get_request_content_type, ARGS_NONE());
  mrb_define_method(mrb, class_request, "content_length=", ap_mrb_set_request_content_length, ARGS_ANY());
  mrb_define_method(mrb, class_request, "content_length", ap_mrb_get_request_content_length, ARGS_NONE());
  mrb_define_method(mrb, class_request, "handler=", ap_mrb_set_request_handler, ARGS_ANY());
  mrb_define_method(mrb, class_request, "handler", ap_mrb_get_request_handler, ARGS_NONE());
  mrb_define_method(mrb, class_request, "content_encoding=", ap_mrb_set_request_content_encoding, ARGS_ANY());
  mrb_define_method(mrb, class_request, "content_encoding", ap_mrb_get_request_content_encoding, ARGS_NONE());
  mrb_define_method(mrb, class_request, "assbackwards", ap_mrb_get_request_assbackwards, ARGS_NONE());
  mrb_define_method(mrb, class_request, "proxyreq=", ap_mrb_set_request_proxyreq, ARGS_ANY());
  mrb_define_method(mrb, class_request, "proxyreq", ap_mrb_get_request_proxyreq, ARGS_NONE());
  mrb_define_method(mrb, class_request, "header_only", ap_mrb_get_request_header_only, ARGS_NONE());
  mrb_define_method(mrb, class_request, "proto_num", ap_mrb_get_request_proto_num, ARGS_NONE());
  mrb_define_method(mrb, class_request, "status=", ap_mrb_set_request_status, ARGS_ANY());
  mrb_define_method(mrb, class_request, "status", ap_mrb_get_request_status, ARGS_NONE());
  mrb_define_method(mrb, class_request, "method_number", ap_mrb_get_request_method_number, ARGS_NONE());
  mrb_define_method(mrb, class_request, "chunked", ap_mrb_get_request_chunked, ARGS_NONE());
  mrb_define_method(mrb, class_request, "read_body", ap_mrb_get_request_read_body, ARGS_NONE());
  mrb_define_method(mrb, class_request, "read_chunked", ap_mrb_get_request_read_chunked, ARGS_NONE());
  mrb_define_method(mrb, class_request, "used_path_info", ap_mrb_get_request_used_path_info, ARGS_NONE());
  mrb_define_method(mrb, class_request, "eos_sent", ap_mrb_get_request_eos_sent, ARGS_NONE());
  mrb_define_method(mrb, class_request, "no_cache", ap_mrb_get_request_no_cache, ARGS_NONE());
  mrb_define_method(mrb, class_request, "no_local_copy", ap_mrb_get_request_no_local_copy, ARGS_NONE());
  // method for loading other class object
  mrb_define_method(mrb, class_request, "notes", ap_mrb_notes_obj, ARGS_NONE());
  mrb_define_method(mrb, class_request, "headers_in", ap_mrb_headers_in_obj, ARGS_NONE());
  mrb_define_method(mrb, class_request, "headers_out", ap_mrb_headers_out_obj, ARGS_NONE());
  mrb_define_method(mrb, class_request, "finfo", ap_mrb_finfo_obj, ARGS_NONE());

  class_notes = mrb_define_class_under(mrb, class_core, "Notes", mrb->object_class);
  mrb_define_method(mrb, class_notes, "[]=", ap_mrb_set_request_notes, ARGS_ANY());
  mrb_define_method(mrb, class_notes, "[]", ap_mrb_get_request_notes, ARGS_ANY());

  class_headers_in = mrb_define_class_under(mrb, class_core, "Headers_in", mrb->object_class);
  mrb_define_method(mrb, class_headers_in, "[]=", ap_mrb_set_request_headers_in, ARGS_ANY());
  mrb_define_method(mrb, class_headers_in, "[]", ap_mrb_get_request_headers_in, ARGS_ANY());
  mrb_define_method(mrb, class_headers_in, "all", ap_mrb_get_request_headers_in_hash, ARGS_ANY());

  class_headers_out = mrb_define_class_under(mrb, class_core, "Headers_out", mrb->object_class);
  mrb_define_method(mrb, class_headers_out, "[]=", ap_mrb_set_request_headers_out, ARGS_ANY());
  mrb_define_method(mrb, class_headers_out, "[]", ap_mrb_get_request_headers_out, ARGS_ANY());
  mrb_define_method(mrb, class_headers_out, "all", ap_mrb_get_request_headers_out_hash, ARGS_ANY());

  class_finfo = mrb_define_class_under(mrb, class_core, "Finfo", mrb->object_class);
  mrb_define_const(mrb,  class_finfo, "APR_NOFILE", mrb_fixnum_value(APR_NOFILE));
  mrb_define_const(mrb,  class_finfo, "APR_REG", mrb_fixnum_value(APR_REG));
  mrb_define_const(mrb,  class_finfo, "APR_DIR", mrb_fixnum_value(APR_DIR));
  mrb_define_const(mrb,  class_finfo, "APR_CHR", mrb_fixnum_value(APR_CHR));
  mrb_define_const(mrb,  class_finfo, "APR_BLK", mrb_fixnum_value(APR_BLK));
  mrb_define_const(mrb,  class_finfo, "APR_PIPE", mrb_fixnum_value(APR_PIPE));
  mrb_define_const(mrb,  class_finfo, "APR_LNK", mrb_fixnum_value(APR_LNK));
  mrb_define_const(mrb,  class_finfo, "APR_SOCK", mrb_fixnum_value(APR_SOCK));
  mrb_define_const(mrb,  class_finfo, "APR_UNKFILE", mrb_fixnum_value(APR_UNKFILE));
  mrb_define_method(mrb, class_finfo, "permission",  ap_mrb_get_request_finfo_protection,  ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "filetype",  ap_mrb_get_request_finfo_filetype,  ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "group",  ap_mrb_get_request_finfo_group,  ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "user",   ap_mrb_get_request_finfo_user,   ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "device",   ap_mrb_get_request_finfo_device, ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "inode",   ap_mrb_get_request_finfo_inode, ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "nlink",   ap_mrb_get_request_finfo_nlink, ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "size",   ap_mrb_get_request_finfo_size,   ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "csize",   ap_mrb_get_request_finfo_csize, ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "atime",  ap_mrb_get_request_finfo_atime,  ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "ctime",  ap_mrb_get_request_finfo_ctime,  ARGS_NONE());
  mrb_define_method(mrb, class_finfo, "mtime",  ap_mrb_get_request_finfo_mtime,  ARGS_NONE());
}

