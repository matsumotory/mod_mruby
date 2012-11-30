#include "ap_mrb_authnprovider.h"
#include "mruby/string.h"
#include "apr_strings.h"

mrb_value mrb_str_new_or_nil(mrb_state *mrb, const char *str) {
    if (str) {
	return mrb_str_new(mrb, str, strlen(str));
    } else {
	return mrb_nil_value();
    }
}


typedef struct authnprovider_rec_t {
    request_rec *r;
    const char *user;
    const char *password;
    const char *realm;
    char *rethash;  
} authnprovider_rec;

authnprovider_rec *mrb_authnprovider_rec_state = NULL;

int ap_mrb_init_authnprovider_basic(request_rec *r, const char* user, const char* password)
{
    mrb_authnprovider_rec_state = (authnprovider_rec *)apr_pcalloc(r->pool, sizeof (*mrb_authnprovider_rec_state));
    mrb_authnprovider_rec_state->r = r;
    mrb_authnprovider_rec_state->user = user;
    mrb_authnprovider_rec_state->password = password;
    mrb_authnprovider_rec_state->realm = NULL;
    mrb_authnprovider_rec_state->rethash = NULL;
    return OK;
}

int ap_mrb_init_authnprovider_digest(request_rec *r, const char* user, const char* realm)
{
    mrb_authnprovider_rec_state = (authnprovider_rec *)apr_pcalloc(r->pool, sizeof (*mrb_authnprovider_rec_state));
    mrb_authnprovider_rec_state->r = r;
    mrb_authnprovider_rec_state->user = user;
    mrb_authnprovider_rec_state->password = NULL;
    mrb_authnprovider_rec_state->realm = realm;
    mrb_authnprovider_rec_state->rethash = NULL;
    return OK;
}

char* ap_mrb_get_authnprovider_digest_rethash()
{
    return mrb_authnprovider_rec_state->rethash;
}

authnprovider_rec* ap_mrb_get_authnprovider()
{
    return mrb_authnprovider_rec_state;
}

mrb_value ap_mrb_get_authnprovider_user(mrb_state *mrb, mrb_value str)
{
    authnprovider_rec *anp = ap_mrb_get_authnprovider();
    return mrb_str_new_or_nil(mrb, anp->user);
}

mrb_value ap_mrb_get_authnprovider_password(mrb_state *mrb, mrb_value str)
{
    authnprovider_rec *anp = ap_mrb_get_authnprovider();
    return mrb_str_new_or_nil(mrb, anp->password);
}

mrb_value ap_mrb_get_authnprovider_realm(mrb_state *mrb, mrb_value str)
{
    authnprovider_rec *anp = ap_mrb_get_authnprovider();
    return mrb_str_new_or_nil(mrb, anp->realm);
}

mrb_value ap_mrb_get_authnprovider_rethash(mrb_state *mrb, mrb_value str)
{
    authnprovider_rec *anp = ap_mrb_get_authnprovider();
    return mrb_str_new_or_nil(mrb, anp->rethash);
}

mrb_value ap_mrb_set_authnprovider_rethash(mrb_state *mrb, mrb_value str)
{
    mrb_value val;
    authnprovider_rec *anp = ap_mrb_get_authnprovider();
    mrb_get_args(mrb, "o", &val);
    anp->rethash = apr_pstrdup(anp->r->pool, RSTRING_PTR(val));
    return val;
}
