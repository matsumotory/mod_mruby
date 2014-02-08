/*
// ap_mrb_init.c - mod_mruby init phase
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"

#define GC_ARENA_RESTORE mrb_gc_arena_restore(mrb, 0);

// base class init functions
void ap_mruby_core_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_server_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_request_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_conn_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_scoreboard_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_env_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_authnprovider_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_filter_init(mrb_state *mrb, struct RClass *class_core);

// add extended class init functions like ap_mruby_redis_init() in lib/redis/redis.c
void ap_mruby_redis_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_hello_init(mrb_state *mrb, struct RClass *class_core);
void ap_mruby_load_init(mrb_state *mrb, struct RClass *class_core);

// init phase for extended calss like lib/redis/redis.c
void ap_mruby_ext_class_init(mrb_state *mrb, struct RClass *class_core)
{
//#ifdef ENABLE_REDIS
//  ap_mruby_redis_init(mrb, class_core); GC_ARENA_RESTORE;
//#endif
#ifdef ENABLE_HELLO
  ap_mruby_hello_init(mrb, class_core); GC_ARENA_RESTORE;
#endif
#ifdef ENABLE_LOAD
  ap_mruby_load_init(mrb, class_core); GC_ARENA_RESTORE;
#endif
}

int ap_mruby_class_init(mrb_state *mrb)
{
  struct RClass *class = mrb_define_class(mrb, "Apache", mrb->object_class);

  ap_mruby_core_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_server_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_scoreboard_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_conn_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_request_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_env_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_authnprovider_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_filter_init(mrb, class); GC_ARENA_RESTORE;
  ap_mruby_ext_class_init(mrb, class);

  return OK;
}
