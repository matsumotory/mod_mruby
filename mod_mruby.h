#ifndef MOD_MURBY_H
#define MOD_MRUBY_H

typedef struct {

    const char *mod_mruby_handler_code;
    const char *mod_mruby_translate_name_first_code;
    const char *mod_mruby_translate_name_middle_code;
    const char *mod_mruby_translate_name_last_code;
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
