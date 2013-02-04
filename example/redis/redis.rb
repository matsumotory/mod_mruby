a = Apache::Request.new()
a.content_type = "text/html"

host    = "127.0.0.1"
port    = 6379
key     = "hoge"

Apache.rputs "> redis connect #{host}: #{port.to_s}<br>"
r = Redis.new host, port

Apache.rputs "> redis set #{key} 200<br>"
r.set key, "200"

Apache.rputs "> redis get #{key}<br>"
Apache.rputs "#{key}: #{r[key]}<br>"

Apache.rputs "> redis set #{key} fuga<br>"
r[key] =  "fuga"

Apache.rputs "> redis get #{key}<br>"
Apache.rputs "#{key}: #{r.get key}<br>"

Apache.rputs "> redis del #{key}<br>"
r.del key

if r[key].nil?
    Apache.rputs "del success!<br>"
end

Apache.rputs "> redis incr #{key}<br>"
Apache.rputs "#{key} incr: #{r.incr(key)}<br>"
Apache.rputs "#{key} incr: #{r.incr(key)}<br>"
Apache.rputs "#{key} incr: #{r.incr(key)}<br>"
Apache.rputs "#{key} incr: #{r.incr(key)}<br>"

Apache.rputs "> redis decr #{key}<br>"
Apache.rputs "#{key} decr: #{r.decr(key)}<br>"
Apache.rputs "#{key} decr: #{r.decr(key)}<br>"
Apache.rputs "#{key} decr: #{r.decr(key)}<br>"
Apache.rputs "#{key} decr: #{r.decr(key)}<br>"

Apache.rputs "> redis lpush logs error<br>"
r.lpush "logs", "error1"
r.lpush "logs", "error2"
r.lpush "logs", "error3"

Apache.rputs "> redis lrange 0 -1<br>"
Apache.rputs r.lrange "logs", 0, -1 + "<br>"

Apache.rputs "> redis ltrim 1 -1<br>"
r.ltrim "logs", 1, -1

Apache.rputs "> redis lrange 0 -1<br>"
Apache.rputs r.lrange "logs", 0, -1

Apache.rputs "> redis del logs<br>"
r.del "logs"

if r["logs"].nil?
    Apache.rputs "del success!<br>"
end

Apache.rputs "> redis publish :one hello<br>"
r.publish "one", "hello"

r.close
