request_rec *mrb_request_rec_state = NULL;

static int ap_mrb_push_request(request_rec *r);
request_rec *ap_mrb_get_request();


static int ap_mrb_push_request(request_rec *r)
{
    mrb_request_rec_state = (request_rec *)apr_pcalloc(r->pool, sizeof (*mrb_request_rec_state));
    mrb_request_rec_state = r;
    return OK;
}

request_rec *ap_mrb_get_request()
{
    return mrb_request_rec_state;
}

