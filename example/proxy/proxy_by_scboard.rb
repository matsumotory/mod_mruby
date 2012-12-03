backends = [
    "http://192.168.0.101:8888/",
    "http://192.168.0.102:8888/",
    "http://192.168.0.103:8888/",
    "http://192.168.0.104:8888/",
]
 
sb = Apache::Scoreboard.new()
busyrate = sb.busy_worker / (sb.process_limit * sb.worker_limit)

if busyrate * 100 > 50
    proxy_rate = backends.length - 2
else if busyrate * 100 > 70
    proxy_rate = backends.length - 1
else if busyrate * 100 > 90
    proxy_rate = backends.length
else
    proxy_rate = backends.length - 3
end
 
r = Apache::Request.new()
 
r.handler  = "proxy-server"
r.proxyreq = Apache::PROXYREQ_REVERSE
r.filename = "proxy:" + backends[rand(proxy_late)] + r.uri
 
Apache::return(Apache::OK)
