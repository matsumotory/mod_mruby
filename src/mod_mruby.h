/*
// mod_mruby.h - mod_mruby core module
//
// See Copyright Notice in LEGAL
*/

#ifndef MOD_MRUBY_H
#define MOD_MRUBY_H

#define MODULE_NAME    "mod_mruby"
#define MODULE_VERSION   "1.4.1"
#define UNSET        -1
#define SET        1
#define ON         1
#define OFF        0

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "mruby.h"
#include "mruby/string.h"
#include "mruby/compile.h"

#undef HAVE_UNISTD_H
#include "http_request.h"
#include "http_log.h"
#include "ap_release.h"

#define CACHE_DISABLE 0
#define CACHE_ENABLE  1

#ifdef __MOD_MRUBY_DEBUG__
#define TRACER ap_log_error(APLOG_MARK , APLOG_NOTICE , 0 , NULL, "%s CHECKING %s" , MODULE_NAME , __func__)
#else
#define TRACER
#endif

#if (AP_SERVER_MINORVERSION_NUMBER > 2)
  #define __APACHE24__
#endif

#if (AP_SERVER_MAJORVERSION_NUMBER == 2)
  #define APLOGNO(n)        "AH" #n ": "
#endif

#ifdef __APACHE24__
  #include "http_main.h"
#else
  #define ap_server_conf      NULL
#endif

enum code_type {
  MOD_MRUBY_STRING,
  MOD_MRUBY_FILE
};

typedef struct {
  union code {
    char *path;
    char *code;
  } code;
  struct RProc *proc;
  mrbc_context *ctx;
  int irep_idx_start;
  int irep_idx_end;
  enum code_type type;
  unsigned int cache;
} mod_mruby_code_t;

typedef struct {

  mod_mruby_code_t *mod_mruby_handler_inline_code;
  mod_mruby_code_t *mod_mruby_handler_first_inline_code;
  mod_mruby_code_t *mod_mruby_handler_middle_inline_code;
  mod_mruby_code_t *mod_mruby_handler_last_inline_code;
  mod_mruby_code_t *mod_mruby_post_read_request_first_inline_code;
  mod_mruby_code_t *mod_mruby_post_read_request_middle_inline_code;
  mod_mruby_code_t *mod_mruby_post_read_request_last_inline_code;
  mod_mruby_code_t *mod_mruby_translate_name_first_inline_code;
  mod_mruby_code_t *mod_mruby_translate_name_middle_inline_code;
  mod_mruby_code_t *mod_mruby_translate_name_last_inline_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_first_inline_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_middle_inline_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_last_inline_code;
  mod_mruby_code_t *mod_mruby_access_checker_first_inline_code;
  mod_mruby_code_t *mod_mruby_access_checker_middle_inline_code;
  mod_mruby_code_t *mod_mruby_access_checker_last_inline_code;
  mod_mruby_code_t *mod_mruby_check_user_id_first_inline_code;
  mod_mruby_code_t *mod_mruby_check_user_id_middle_inline_code;
  mod_mruby_code_t *mod_mruby_check_user_id_last_inline_code;
  mod_mruby_code_t *mod_mruby_auth_checker_first_inline_code;
  mod_mruby_code_t *mod_mruby_auth_checker_middle_inline_code;
  mod_mruby_code_t *mod_mruby_auth_checker_last_inline_code;
  mod_mruby_code_t *mod_mruby_fixups_first_inline_code;
  mod_mruby_code_t *mod_mruby_fixups_middle_inline_code;
  mod_mruby_code_t *mod_mruby_fixups_last_inline_code;
  mod_mruby_code_t *mod_mruby_log_transaction_first_inline_code;
  mod_mruby_code_t *mod_mruby_log_transaction_middle_inline_code;
  mod_mruby_code_t *mod_mruby_log_transaction_last_inline_code;

  mod_mruby_code_t *mod_mruby_handler_code;
  mod_mruby_code_t *mod_mruby_handler_first_code;
  mod_mruby_code_t *mod_mruby_handler_middle_code;
  mod_mruby_code_t *mod_mruby_handler_last_code;
  mod_mruby_code_t *mod_mruby_post_read_request_first_code;
  mod_mruby_code_t *mod_mruby_post_read_request_middle_code;
  mod_mruby_code_t *mod_mruby_post_read_request_last_code;
  mod_mruby_code_t *mod_mruby_translate_name_first_code;
  mod_mruby_code_t *mod_mruby_translate_name_middle_code;
  mod_mruby_code_t *mod_mruby_translate_name_last_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_first_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_middle_code;
  mod_mruby_code_t *mod_mruby_map_to_storage_last_code;
  mod_mruby_code_t *mod_mruby_access_checker_first_code;
  mod_mruby_code_t *mod_mruby_access_checker_middle_code;
  mod_mruby_code_t *mod_mruby_access_checker_last_code;
  mod_mruby_code_t *mod_mruby_check_user_id_first_code;
  mod_mruby_code_t *mod_mruby_check_user_id_middle_code;
  mod_mruby_code_t *mod_mruby_check_user_id_last_code;
  mod_mruby_code_t *mod_mruby_auth_checker_first_code;
  mod_mruby_code_t *mod_mruby_auth_checker_middle_code;
  mod_mruby_code_t *mod_mruby_auth_checker_last_code;
  mod_mruby_code_t *mod_mruby_fixups_first_code;
  mod_mruby_code_t *mod_mruby_fixups_middle_code;
  mod_mruby_code_t *mod_mruby_fixups_last_code;
  mod_mruby_code_t *mod_mruby_log_transaction_first_code;
  mod_mruby_code_t *mod_mruby_log_transaction_middle_code;
  mod_mruby_code_t *mod_mruby_log_transaction_last_code;
  mod_mruby_code_t *mod_mruby_authn_check_password_code;
  mod_mruby_code_t *mod_mruby_authn_get_realm_hash_code;
  mod_mruby_code_t *mod_mruby_output_filter_code;

} mruby_dir_config_t;

typedef struct {

  mod_mruby_code_t *mod_mruby_quick_handler_first_code;
  mod_mruby_code_t *mod_mruby_quick_handler_middle_code;
  mod_mruby_code_t *mod_mruby_quick_handler_last_code;
  mod_mruby_code_t *mod_mruby_post_config_first_code;
  mod_mruby_code_t *mod_mruby_post_config_middle_code;
  mod_mruby_code_t *mod_mruby_post_config_last_code;
  mod_mruby_code_t *mod_mruby_child_init_first_code;
  mod_mruby_code_t *mod_mruby_child_init_middle_code;
  mod_mruby_code_t *mod_mruby_child_init_last_code;
  mod_mruby_code_t *mod_mruby_insert_filter_first_code;
  mod_mruby_code_t *mod_mruby_insert_filter_middle_code;
  mod_mruby_code_t *mod_mruby_insert_filter_last_code;
  int mruby_cache_table_size;

} mruby_config_t;

typedef struct cache_code_str {

  int cache_id;
  int ireq_id;
  char *filename;
  mrb_state *mrb;
  int stat_mtime;

} cache_code_t;

typedef struct cache_shm_table {

  cache_code_t *cache_code_slot;

} cache_table_t;


#endif
