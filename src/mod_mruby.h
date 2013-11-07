/*
** mod_mruby - A Web Server Extension Mechanism Using mruby
**
** Copyright (c) mod_mruby developers 2012-
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

#ifndef MOD_MURBY_H
#define MOD_MRUBY_H

#define MODULE_NAME        "mod_mruby"
#define MODULE_VERSION     "0.9.4"
#define UNSET              -1
#define SET                1
#define ON                 1
#define OFF                0

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "mruby.h"
#include "mruby/string.h"

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
    #define APLOGNO(n)              "AH" #n ": "
#endif

#ifdef __APACHE24__
    #include "http_main.h"
#else
    extern server_rec *ap_server_conf;
#endif

enum code_type {
    MOD_MRUBY_STRING,
    MOD_MRUBY_FILE
};

typedef struct {
    union {
        char *path;
        char *code;
    };
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
