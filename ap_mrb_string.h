
//mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str, const char *msg, request_rec *r);
//mrb_value ap_mrb_rputs_test(mrb_state *mrb, mrb_value str, request_rec *r);

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

