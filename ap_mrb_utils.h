
mrb_value ap_mrb_sleep(mrb_state *mrb, mrb_value str);


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

