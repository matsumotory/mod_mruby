/*
// load.c - load name class for mod_mruby
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_request.h"
#include "mruby/hash.h"

#ifdef ENABLE_LOAD

static double current_load(int i)
{
    double cload[3];

    cload[0] = cload[1] = cload[2] = -1;
    getloadavg(cload, 3);

    return cload[i];
}

mrb_value ap_mrb_get_loadavg(mrb_state *mrb, mrb_value self)
{
    int i;
    mrb_value hash = mrb_hash_new(mrb);

    mrb_hash_set(mrb, hash, mrb_str_new2(mrb, "1"),  mrb_float_value(current_load(0)));
    mrb_hash_set(mrb, hash, mrb_str_new2(mrb, "5"),  mrb_float_value(current_load(1)));
    mrb_hash_set(mrb, hash, mrb_str_new2(mrb, "15"), mrb_float_value(current_load(2)));

    return hash;
}

// Add init function to ap_mruby_ext_calss_init() in ap_mrb_init.c
// and prototype to ap_mrb_init.c
void ap_mruby_load_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_load;

    class_load = mrb_define_class_under(mrb, class_core, "Load", mrb->object_class);
    mrb_define_method(mrb, class_load, "getloadavg", ap_mrb_get_loadavg, ARGS_NONE());
}

#endif
