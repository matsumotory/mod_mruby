r = Apache::Request.new

unless r.headers_in["X-REQUEST-HEADER"].nil?
  r.headers_out["X-RESPONSE-HEADER"] = r.headers_in["X-REQUEST-HEADER"]
  Apache.rputs "X-REQUEST-HEADER found"
else
  r.headers_out["X-RESPONSE-HEADER"] = "nothing"
  Apache.rputs "X-REQUEST-HEADER not found"
end
