/*
// ap_mrb_scoreboard.c - to provide scoreboard methods
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include "ap_mrb_scoreboard.h"
#include "mruby/hash.h"
#include "scoreboard.h"
#include "ap_mpm.h"
#include <unistd.h>

#define KBYTE 1024
#define MBYTE 1048576L
#define GBYTE 1073741824L

#ifdef __APACHE24__
#define ap_get_scoreboard_worker ap_get_scoreboard_worker_from_indexes
#endif

#if !defined(__APACHE24__) && defined(_WIN32)
/*
 * libhttpd.dll does not export following variables.
 * This won't work correctly, but working well for other functional.
 */
int ap_extended_status = 0;
ap_generation_t volatile ap_my_generation = 0;
scoreboard *ap_scoreboard_image = NULL;
#endif

static int mruby_server_limit, mruby_thread_limit;

static apr_off_t sb_get_kbcount();
static unsigned long sb_get_access_count();
static apr_time_t sb_get_restart_time();
static apr_interval_time_t sb_get_uptime();
static int sb_get_idle_worker();
static int sb_get_process_worker();

static apr_time_t sb_get_restart_time()
{
    if (!ap_extended_status)
        return 0;
    return ap_scoreboard_image->global->restart_time;
}

static apr_interval_time_t sb_get_uptime()
{
    apr_time_t nowtime;
    apr_interval_time_t up_time;

    if (!ap_extended_status)
        return 0;

    nowtime = apr_time_now();
    up_time = (apr_uint32_t)apr_time_sec(nowtime - ap_scoreboard_image->global->restart_time);

    return up_time;
}

static int sb_get_process_worker()
{
    int i, j, res;
    worker_score *ws_record;
    process_score *ps_record;

#ifdef __APACHE24__
    ap_generation_t ap_my_generation;
    ap_mpm_query(AP_MPMQ_GENERATION, &ap_my_generation);
#endif

    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    int process = 0;
    if (!ap_extended_status)
        return process;

    for (i = 0; i < mruby_server_limit; ++i) {
        ps_record = ap_get_scoreboard_process(i);
        for (j = 0; j < mruby_thread_limit; ++j) {
            ws_record = ap_get_scoreboard_worker(i, j);
            res = ws_record->status;
            if (!ps_record->quiescing && ps_record->pid) {
                if (res == SERVER_READY && ps_record->generation == ap_my_generation)
                    continue;
                else if (res != SERVER_DEAD && res != SERVER_STARTING && res != SERVER_IDLE_KILL)
                    process++;
            }
        }
    }

    return process;
}

static int sb_get_idle_worker()
{
    int i, j, res;
    worker_score *ws_record;
    process_score *ps_record;

#ifdef __APACHE24__
    ap_generation_t ap_my_generation;
    ap_mpm_query(AP_MPMQ_GENERATION, &ap_my_generation);
#endif

    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    int idle = 0;
    if (!ap_extended_status)
        return idle;

    for (i = 0; i < mruby_server_limit; ++i) {
        ps_record = ap_get_scoreboard_process(i);
        for (j = 0; j < mruby_thread_limit; ++j) {
            ws_record = ap_get_scoreboard_worker(i, j);
            res = ws_record->status;
            if (!ps_record->quiescing && ps_record->pid) {
                if (res == SERVER_READY && ps_record->generation == ap_my_generation)
                    idle++;
            }
        }
    }

    return idle;
}

static apr_off_t sb_get_kbcount()
{
    int i, j, res;
    unsigned long lres;
    apr_off_t bytes;
    apr_off_t bcount, kbcount;

    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    worker_score *ws_record;
    process_score *ps_record;

    bcount = 0;
    kbcount = 0;

   if (!ap_extended_status)
        return kbcount;

    for (i = 0; i < mruby_server_limit; ++i) {
        ps_record = ap_get_scoreboard_process(i);
        for (j = 0; j < mruby_thread_limit; ++j) {

            ws_record = ap_get_scoreboard_worker(i, j);

            res = ws_record->status;

            lres = ws_record->access_count;
            bytes = ws_record->bytes_served;

            if (lres != 0 || (res != SERVER_READY && res != SERVER_DEAD)) {
                bcount += bytes;

                if (bcount >= KBYTE) {
                    kbcount += (bcount >> 10);
                    bcount = bcount & 0x3ff;
                }
            }
        }
    }
    return kbcount;
}

static unsigned long sb_get_access_count()
{
    int i, j, res;
    unsigned long count;
    unsigned long lres;

    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &mruby_thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &mruby_server_limit);

    worker_score *ws_record;
    process_score *ps_record;

    count = 0;

   if (!ap_extended_status)
        return count;

    for (i = 0; i < mruby_server_limit; ++i) {
        ps_record = ap_get_scoreboard_process(i);
        for (j = 0; j < mruby_thread_limit; ++j) {

            ws_record = ap_get_scoreboard_worker(i, j);

            res = ws_record->status;
            lres = ws_record->access_count;

            if (lres != 0 || (res != SERVER_READY && res != SERVER_DEAD))
                count += lres;
        }
    }
    return count;
}

mrb_value ap_mrb_get_scoreboard_idle_worker(mrb_state *mrb, mrb_value str)
{
    return mrb_fixnum_value((mrb_int)sb_get_idle_worker());
}

mrb_value ap_mrb_get_scoreboard_process_worker(mrb_state *mrb, mrb_value str)
{
    return mrb_fixnum_value((mrb_int)sb_get_process_worker());
}

mrb_value ap_mrb_get_scoreboard_restart_time(mrb_state *mrb, mrb_value str)
{
    return mrb_float_value((mrb_float)sb_get_restart_time());
}

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
                return mrb_fixnum_value((mrb_int)ws_record->access_count);
        }
    }
    return mrb_fixnum_value(-1);
}

mrb_value ap_mrb_get_scoreboard_uptime(mrb_state *mrb, mrb_value str)
{
    return mrb_float_value((mrb_float)sb_get_uptime());
}

mrb_value ap_mrb_get_scoreboard_total_kbyte(mrb_state *mrb, mrb_value str)
{
    return mrb_float_value((mrb_float)sb_get_kbcount());
}

mrb_value ap_mrb_get_scoreboard_total_access(mrb_state *mrb, mrb_value str)
{
    return mrb_float_value((mrb_float)sb_get_access_count());
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

void ap_mruby_scoreboard_init(mrb_state *mrb, struct RClass *class_core)
{
    struct RClass *class_scoreboard;

    class_scoreboard = mrb_define_class_under(mrb, class_core, "Scoreboard", mrb->object_class);
    mrb_define_method(mrb, class_scoreboard, "status", ap_mrb_get_scoreboard_status, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "counter", ap_mrb_get_scoreboard_counter, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "pid", ap_mrb_get_scoreboard_pid, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "server_limit", ap_mrb_get_scoreboard_server_limit, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "thread_limit", ap_mrb_get_scoreboard_thread_limit, ARGS_NONE());
    mrb_define_method(mrb, class_scoreboard, "access_counter", ap_mrb_get_scoreboard_access_counter, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "total_kbyte", ap_mrb_get_scoreboard_total_kbyte, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "total_access", ap_mrb_get_scoreboard_total_access, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "uptime", ap_mrb_get_scoreboard_uptime, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "restart_time", ap_mrb_get_scoreboard_restart_time, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "idle_worker", ap_mrb_get_scoreboard_idle_worker, ARGS_ANY());
    mrb_define_method(mrb, class_scoreboard, "busy_worker", ap_mrb_get_scoreboard_process_worker, ARGS_ANY());
}
