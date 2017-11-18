r = Apache::Request.new

r.headers_out["Set-Cookie"] = "hoge1"
r.headers_out["Set-Cookie"] = "hoge2"
r.headers_out["X-RESPONSE-HEADER"] = "hogehoge"

unless r.headers_in["X-REQUEST-HEADER"].nil?
  r.headers_out["X-RESPONSE-HEADER"] = r.headers_in["X-REQUEST-HEADER"]
  Apache.rputs "X-REQUEST-HEADER found"
else
  r.headers_out["X-RESPONSE-HEADER"] = "nothing"
  Apache.rputs "X-REQUEST-HEADER not found"
end
