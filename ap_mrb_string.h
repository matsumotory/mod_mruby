#include <mruby.h>
#include <mruby/proc.h>
#include <compile.h>

mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);
//mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str, const char *msg, request_rec *r);
//mrb_value ap_mrb_rputs_test(mrb_state *mrb, mrb_value str, request_rec *r);


mrb_value
ap_mrb_sleep(mrb_state *mrb, mrb_value str) {

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 1) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "argument is 1");
    }

    sleep(mrb_fixnum(argv[0]));

    return str;
}

/*
mrb_value
ap_mrb_rputs(mrb_state *mrb, mrb_value str, const char *msg, request_rec *r) {

    ap_rputs(msg, r); 

    return mrb_nil_value();
}

mrb_value
ap_mrb_rputs_test(mrb_state *mrb, mrb_value str, request_rec *r) {

    ap_rputs("hello", r); 

    return mrb_nil_value();
}
*/

