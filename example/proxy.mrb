backends = [
    "http://192.168.0.101:8888/",
    "http://192.168.0.102:8888/",
    "http://192.168.0.103:8888/",
    "http://192.168.0.104:8888/",
]
 
# この辺に色々条件を入れたり、backends配列から取り出すルールを別途定義したりするとmod_mrubyのうまみがでる？
 
r = Apache::Request.new()
 
r.handler  = "proxy-server"
r.proxyreq = Apache::PROXYREQ_REVERSE
r.filename = "proxy:" + backends[rand(backends.length)] + r.uri
 
Apache::return(Apache::OK)
