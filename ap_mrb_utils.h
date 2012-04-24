#include <mruby/string.h>
#include <unistd.h>

mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_err_logger(mrb_state *mrb, mrb_value str);


mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str)
{

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 1) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "argument is 1");
    }

    sleep(mrb_fixnum(argv[0]));

    return str;
}

mrb_value ap_mrb_errlogger(mrb_state *mrb, mrb_value str)
{

    struct RProc *b;
    mrb_value argc, *argv;
    
    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "argument is 2");
    }

    ap_log_error(APLOG_MARK, mrb_fixnum(argv[0]), 0, NULL, RSTRING_PTR(argv[1]));

    return str;
}
