/*
// ap_mrb_redis.c - to provide redis methods
//
// See Copyright Notice in mod_mruby.h
*/

#include "mod_mruby.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ap_mrb_redis.h"

mrb_value ap_mrb_redis_connect(mrb_state *mrb, mrb_value self)
{
    mrb_value host, port;

    mrb_get_args(mrb, "oo", &host, &port);
    //request_rec *r = ap_mrb_get_request();

    redisContext *rc = redisConnect(RSTRING_PTR(host), mrb_fixnum(port));
    if (rc->err) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0
            , NULL
            , "%s ERROR %s: redis connect faild"
            , MODULE_NAME
            , __func__
        );
        return mrb_nil_value();
    } else {
        mrb_iv_set(mrb, self, mrb_intern(mrb, "redis_c"), mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, NULL, (void*) rc)));
        return self;
    }
}

mrb_value ap_mrb_redis_set(mrb_state *mrb, mrb_value self)
{
    mrb_value key, val;
    redisContext *rc;
    mrb_value context;

    mrb_get_args(mrb, "oo", &key, &val);

    context = mrb_iv_get(mrb, self, mrb_intern(mrb, "redis_c"));
    Data_Get_Struct(mrb, context, NULL, rc);
    if (!rc) {
        ap_log_error(APLOG_MARK
            , APLOG_ERR
            , 0
            , NULL
            , "%s ERROR %s: mrb_iv_get redis_c failed"
            , MODULE_NAME
            , __func__
        );
    }

    redisReply *rs;
    rs = redisCommand(rc,"set %s %s", RSTRING_PTR(key), RSTRING_PTR(val));
    freeReplyObject(rs);

    return  self;
}

mrb_value ap_mrb_redis_get(mrb_state *mrb, mrb_value self)
{
    mrb_value key;
    redisContext *rc;
    mrb_value context;
    char *val;
    request_rec *r = ap_mrb_get_request();

    mrb_get_args(mrb, "o", &key);

    context = mrb_iv_get(mrb, self, mrb_intern(mrb, "redis_c"));
    Data_Get_Struct(mrb, context, NULL, rc);

    redisReply *rs;
    rs = redisCommand(rc,"get %s", RSTRING_PTR(key));
    if (rs->type == REDIS_REPLY_STRING) {
        val = apr_pstrdup(r->pool, rs->str);
        freeReplyObject(rs);
        return mrb_str_new(mrb, val, strlen(val));
    } else {
        return mrb_nil_value();
    }
}

