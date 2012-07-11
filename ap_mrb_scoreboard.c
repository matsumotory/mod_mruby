#include "mod_mruby.h"
#include "ap_mrb_scoreboard.h"
#include "mruby/hash.h"
#include "scoreboard.h"
#include "ap_mpm.h"

mrb_value ap_mrb_get_scoreboard_status(mrb_state *mrb, mrb_value str)
{

    int i, j;
    int mruby_server_limit, mruby_thread_limit;
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


/*
mrb_value ap_mrb_get_conn_remote_host(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->connection->remote_host));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_conn_remote_logname(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->connection->remote_logname));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_conn_local_ip(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->connection->local_ip));
    return mrb_str_new(mrb, val, strlen(val));
}

mrb_value ap_mrb_get_conn_local_host(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    char *val = apr_pstrdup(r->pool, ap_mrb_string_check(r->pool, r->connection->local_host));
    return mrb_str_new(mrb, val, strlen(val));
}

// int write
mrb_value ap_mrb_set_server_loglevel(mrb_state *mrb, mrb_value str)
{
    mrb_int val;
    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "i", &val);
#ifdef __APACHE24__
    r->server->log.level = (int)val;
#else
    r->server->loglevel = (int)val;
#endif
    return str;
}

// int read
mrb_value ap_mrb_get_server_loglevel(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
#ifdef __APACHE24__
    return mrb_fixnum_value(r->server->log.level);
#else
    return mrb_fixnum_value(r->server->loglevel);
#endif
}

mrb_value ap_mrb_get_conn_keepalives(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    return mrb_fixnum_value(r->connection->keepalives);
}

mrb_value ap_mrb_get_conn_data_in_input_filters(mrb_state *mrb, mrb_value str)
{
    request_rec *r = ap_mrb_get_request();
    return mrb_fixnum_value(r->connection->data_in_input_filters);
}

*/
