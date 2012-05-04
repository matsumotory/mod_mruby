#include "ap_mrb_string.h"

mrb_value ap_mrb_rputs(mrb_state *mrb, mrb_value str)
{
    mrb_value msg;

    mrb_get_args(mrb, "o", &msg);
    ap_rputs(RSTRING_PTR(msg), ap_mrb_get_request()); 

    return str;
}
