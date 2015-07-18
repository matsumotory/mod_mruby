#ifndef AP_MRB_AUTHNPROVIDER_H
#define AP_MRB_AUTHNPROVIDER_H

#include "mruby.h"

#undef HAVE_UNISTD_H
#include "http_request.h"
#include "mod_auth.h"

int ap_mrb_init_authnprovider_basic(request_rec *r, const char *user,
                                    const char *password);
int ap_mrb_init_authnprovider_digest(request_rec *r, const char *user,
                                     const char *realm);

char *ap_mrb_get_authnprovider_digest_rethash();

#endif
