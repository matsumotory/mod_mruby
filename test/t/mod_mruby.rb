##
# mod_mruby test

base = 'http://127.0.0.1'

assert('mod_mruby', 'location /hello-inline') do
  res = HttpRequest.new.get base + '/hello-inline'
  assert_equal 'hello', res["body"]
end

assert('mod_mruby', 'location /echo-test') do
  res = HttpRequest.new.get base + '/echo-test'
  assert_equal "echo\n", res["body"]
end

assert('mod_mruby', 'location /proxy') do
  res = HttpRequest.new.get base + '/proxy'
  assert_equal "proxy reply success", res["body"]
end

assert('mod_mruby', 'output filter') do
  res = HttpRequest.new.get base + '/'
  body_ary = res["body"].split("\n")
  head = body_ary.first
  tail = body_ary.last
  assert_equal "__mod_mruby_head__", head
  assert_equal "__mod_mruby_tail__", tail
end

assert('mod_mruby', 'location /header') do
  res1 = HttpRequest.new.get base + '/header'
  res2 = HttpRequest.new.get base + '/header', nil, {"X-REQUEST-HEADER" => "hoge"}

  assert_equal "X-REQUEST-HEADER not found", res1["body"]
  assert_equal "nothing", res1["x-response-header"]
  assert_equal "X-REQUEST-HEADER found", res2["body"]
  assert_equal "hoge", res2["x-response-header"]
end

assert('mod_mruby', 'location /service_unavailable') do
  res = HttpRequest.new.get base + '/service_unavailable'
  assert_equal 503, res.code
end

