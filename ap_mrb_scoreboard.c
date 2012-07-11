#include "mod_mruby.h"
#include "ap_mrb_scoreboard.h"
#include "mruby/hash.h"
#include "scoreboard.h"
#include "ap_mpm.h"
#include <unistd.h>

static int mruby_server_limit, mruby_thread_limit;

mrb_value ap_mrb_get_scoreboard_pid(mrb_state *mrb, mrb_value str)
{
    return mrb_fixnum_value(getpid());
}

mrb_value ap_mrb_get_scoreboard_server_limit(mrb_state *mrb, mrb_value str)
{
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);
    return mrb_fixnum_value(mruby_server_limit);
}

mrb_value ap_mrb_get_scoreboard_thread_limit(mrb_state *mrb, mrb_value str)
{
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    return mrb_fixnum_value(mruby_thread_limit);
}

mrb_value ap_mrb_get_scoreboard_access_counter(mrb_state *mrb, mrb_value str)
{
    int i, j;
    mrb_int pid;
    mrb_get_args(mrb, "i", &pid);
    worker_score *ws_record;
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);
    for (i = 0; i < mruby_server_limit; ++i) {
        for (j = 0; j < mruby_thread_limit; ++j) {
            ws_record = ap_get_scoreboard_worker(i, j);
            if ((int)ws_record->pid == (int)pid)
                return mrb_fixnum_value((int)ws_record->access_count);
        }
    }
    return mrb_fixnum_value(-1);
}

mrb_value ap_mrb_get_scoreboard_status(mrb_state *mrb, mrb_value str)
{

    int i, j;
    worker_score *ws_record;
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    mrb_value hash = mrb_hash_new(mrb);

    for (i = 0; i < mruby_server_limit; ++i) {
        for (j = 0; j < mruby_thread_limit; ++j) {
            ws_record = ap_get_scoreboard_worker(i, j);

            switch (ws_record->status) {
                case SERVER_BUSY_READ:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_BUSY_READ", strlen("SERVER_BUSY_READ"))
                    );
                case SERVER_BUSY_WRITE:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_BUSY_WRITE", strlen("SERVER_BUSY_WRITE"))
                    );
                case SERVER_BUSY_KEEPALIVE:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_BUSY_KEEPALIVE", strlen("SERVER_BUSY_KEEPALIVE"))
                    );
                case SERVER_BUSY_LOG:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_BUSY_LOG", strlen("SERVER_BUSY_LOG"))
                    );
                case SERVER_BUSY_DNS:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_BUSY_DNS", strlen("SERVER_BUSY_DNS"))
                    );
                case SERVER_CLOSING:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_CLOSING", strlen("SERVER_CLOSING"))
                    );
                case SERVER_GRACEFUL:
                    mrb_hash_set(mrb
                        , hash
                        , mrb_str_new(mrb, ws_record->client, strlen(ws_record->client))
                        , mrb_str_new(mrb, "SERVER_GRACEFUL", strlen("SERVER_GRACEFUL"))
                    );

                default:
                    break;
            }
        }
    }

    return hash;
}

mrb_value ap_mrb_get_scoreboard_counter(mrb_state *mrb, mrb_value str)
{

    int i, j;
    worker_score *ws_record;
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    int bread   = 0;
    int bwrite  = 0;
    int bkeep   = 0;
    int blog    = 0;
    int bdns    = 0;
    int close   = 0;
    int grace   = 0;

    mrb_value hash = mrb_hash_new(mrb);

    for (i = 0; i < mruby_server_limit; ++i) {
        for (j = 0; j < mruby_thread_limit; ++j) {
            ws_record = ap_get_scoreboard_worker(i, j);

            switch (ws_record->status) {
                case SERVER_BUSY_READ:
                    bread++;
                case SERVER_BUSY_WRITE:
                    bwrite++;
                case SERVER_BUSY_KEEPALIVE:
                    bkeep++;
                case SERVER_BUSY_LOG:
                    blog++;
                case SERVER_BUSY_DNS:
                    bdns++;
                case SERVER_CLOSING:
                    close++;
                case SERVER_GRACEFUL:
                    grace++;
                default:
                    break;
            }
        }
    }
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_BUSY_READ", strlen("SERVER_BUSY_READ"))
        , mrb_fixnum_value(bread)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_BUSY_WRITE", strlen("SERVER_BUSY_WRITE"))
        , mrb_fixnum_value(bwrite)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_BUSY_KEEPALIVE", strlen("SERVER_BUSY_KEEPALIVE"))
        , mrb_fixnum_value(bkeep)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_BUSY_LOG", strlen("SERVER_BUSY_LOG"))
        , mrb_fixnum_value(blog)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_BUSY_DNS", strlen("SERVER_BUSY_DNS"))
        , mrb_fixnum_value(bdns)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_CLOSING", strlen("SERVER_CLOSING"))
        , mrb_fixnum_value(close)
    );
    mrb_hash_set(mrb
        , hash
        , mrb_str_new(mrb, "SERVER_GRACEFUL", strlen("SERVER_GRACEFUL"))
        , mrb_fixnum_value(grace)
    );

    return hash;
}

