#ifndef AP_MRB_AUTHNPROVIDER_H
#define AP_MRB_AUTHNPROVIDER_H

#include "mruby.h"
#include "http_request.h"
#include "mod_auth.h"

int ap_mrb_init_authnprovider_basic(request_rec *r, const char* user, const char* password);
int ap_mrb_init_authnprovider_digest(request_rec *r, const char* user, const char* realm);

char* ap_mrb_get_authnprovider_digest_rethash();

mrb_value ap_mrb_get_authnprovider_user(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_authnprovider_password(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_authnprovider_realm(mrb_state *mrb, mrb_value str);
mrb_value ap_mrb_get_authnprovider_rethash(mrb_state *mrb, mrb_value str);

mrb_value ap_mrb_set_authnprovider_rethash(mrb_state *mrb, mrb_value str);

#endif
