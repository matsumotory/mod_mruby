backend = "http://127.0.0.1:8081"
uri = "/proxy-reply"
r = Apache::Request.new

r.handler  = "proxy-server"
r.proxyreq = Apache::PROXYREQ_REVERSE
r.filename = "proxy:" + backend + uri
