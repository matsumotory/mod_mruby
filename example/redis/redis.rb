r = Apache::Request.new()
r.content_type = "text/html"

a       = Apache
host    = "127.0.0.1"
port    = 6379

a.rputs("> redis connect " + host + ":" + port.to_s + "<br>")
redis = Apache::Redis.new(host, port)

key     = "hoge"
val     = "100"

a.rputs("> redis set " + key + " " + val + "<br>")
redis.set(key, val)

a.rputs("> redis get " + key + "<br>")
a.rputs(key + ": " + redis.get(key) + "<br><br>")

key     = "hoge"
val     = "bbbbbbbbbbbbbb"

a.rputs("> redis set " + key + " " + val + "<br>")
redis.set(key, val)

a.rputs("> redis get " + key + "<br>")
a.rputs(key + ": " + redis.get(key))
