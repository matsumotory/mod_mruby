/*
// hello.c - hello name class for mod_mruby
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_request.h"

#ifdef ENABLE_HELLO

mrb_value ap_mrb_hello_name(mrb_state *mrb, mrb_value self)
{
    mrb_value name;
    request_rec *r = ap_mrb_get_request();

    mrb_get_args(mrb, "o", &name);
    char *hello_string = apr_psprintf(r->pool, "hello %s-san", RSTRING_PTR(name));

    return mrb_str_new2(mrb, hello_string);
}

// Add init function to ap_mruby_ext_calss_init() in ap_mrb_init.c
// and prototype to ap_mrb_init.c
void ap_mruby_hello_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_hello;

    class_hello = mrb_define_class_under(mrb, class_core, "Hello", mrb->object_class);
    mrb_define_method(mrb, class_hello, "hello", ap_mrb_hello_name, ARGS_ANY());
}

#endif
