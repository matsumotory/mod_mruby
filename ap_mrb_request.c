#include "mod_mruby.h"
#include "ap_mrb_init.h"
#include "ap_mrb_request.h"
#include "json.h"


static struct mrb_data_type request_rec_type = {
    "request_rec", 0,
};


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

const char *ap_mrb_string_check(apr_pool_t *p, const char *str)
{
    char *val;

    if (str == NULL) {
        val = apr_pstrdup(p, "null");
        return val;
    }

    return str;
}

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


mrb_value ap_mrb_get_request_the_request(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->the_request));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_protocol(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->protocol));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_vlist_validator(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->vlist_validator));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_user(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->user));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_ap_auth_type(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->ap_auth_type));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_unparsed_uri(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->unparsed_uri));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_uri(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->uri));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_filename(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->filename));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_canonical_filename(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->canonical_filename));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_path_info(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->path_info));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_args(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->args));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_hostname(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool,r->hostname));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_status_line(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool,r->status_line));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_method(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool,r->method));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_range(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool,r->range));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_content_type(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->content_type));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_handler(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->handler));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_content_encoding(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->content_encoding));
    return mrb_str_new(mrb, val, strlen(val));
}


mrb_value ap_mrb_set_request_the_request(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->the_request = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_protocol(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->protocol = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_vlist_validator(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->vlist_validator = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_user(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->user = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_ap_auth_type(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->ap_auth_type = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_unparsed_uri(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->unparsed_uri = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_uri(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->uri = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->filename = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_canonical_filename(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->canonical_filename = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_path_info(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->path_info = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_args(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->args = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_hostname(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->hostname = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_status_line(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->status_line = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_method(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->method = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_range(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->range = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_content_type(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->content_type = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_handler(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->handler = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_set_request_content_encoding(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);
    r->content_encoding = apr_pstrdup(r->pool, RSTRING_PTR(val));
    return val;
}


mrb_value ap_mrb_set_request_readers_in(mrb_state *mrb, mrb_value str)
{
    mrb_value key, val;

    mrb_get_args(mrb, "oo", &key, &val);
    request_rec *r = ap_mrb_get_request();
    apr_table_set(r->headers_in, RSTRING_PTR(key), RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_get_request_readers_in(mrb_state *mrb, mrb_value str)
{
    mrb_value key;

    mrb_get_args(mrb, "o", &key);
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_table_get(r->headers_in, RSTRING_PTR(key));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_set_request_readers_out(mrb_state *mrb, mrb_value str)
{
    mrb_value key, val;

    mrb_get_args(mrb, "oo", &key, &val);
    request_rec *r = ap_mrb_get_request();
    apr_table_set(r->headers_out, RSTRING_PTR(key), RSTRING_PTR(val));
    return val;
}

mrb_value ap_mrb_get_request_readers_out(mrb_state *mrb, mrb_value str)
{
    mrb_value key;

    mrb_get_args(mrb, "o", &key);
    request_rec *r = ap_mrb_get_request();
    const char *val = apr_table_get(r->headers_out, RSTRING_PTR(key));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_request_assbackwards(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->assbackwards;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_set_request_proxyreq(mrb_state *mrb, mrb_value str)
{

    mrb_int ret;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "i", &ret);
    r->proxyreq = (int)ret;

    return str;
}

mrb_value ap_mrb_get_request_proxyreq(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->proxyreq;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_header_only(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->header_only;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_proto_num(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->proto_num;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_status(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->status;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_method_number(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->method_number;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_chunked(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->chunked;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_read_body(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->read_body;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_read_chunked(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->read_chunked;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_used_path_info(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->used_path_info;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_eos_sent(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->eos_sent;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_no_cache(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->no_cache;
    return mrb_fixnum_value(val);
}

mrb_value ap_mrb_get_request_no_local_copy(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    int val = r->no_local_copy;
    return mrb_fixnum_value(val);
}


mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value argc, *argv;
    char *member, *value;
    request_rec *r = ap_mrb_get_request();

    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 2"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    member = RSTRING_PTR(argv[0]);
    value  = RSTRING_PTR(argv[1]);

    if (strcmp(member, "filename") == 0)
        r->filename = apr_pstrdup(r->pool, value);
    else if (strcmp(member, "uri") == 0)
        r->uri = apr_pstrdup(r->pool, value);

    return str;
}
