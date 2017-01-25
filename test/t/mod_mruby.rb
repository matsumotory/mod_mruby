##
# mod_mruby test

def base64 value
  r = [ value ].pack 'm'
  r.include?("\n") ? r.split("\n").join("") : r
end

def server_addr
  "127.0.0.1:38080"
end

def base
  "http://#{server_addr}"
end

t = SimpleTest.new "mod_mruby test"

server_version = HttpRequest.new.get(base + '/server-version')["body"]

t.assert('mod_mruby', 'location /hello-inline') do
  res = HttpRequest.new.get base + '/hello-inline'
  t.assert_equal 'hello', res["body"]
end

t.assert('mod_mruby', 'location /echo-test') do
  res = HttpRequest.new.get base + '/echo-test'
  t.assert_equal "echo\n", res["body"]
end

t.assert('mod_mruby', 'location /proxy') do
  res = HttpRequest.new.get base + '/proxy'
  t.assert_equal "proxy reply success", res["body"]
end

t.assert('mod_mruby', 'output filter') do
  res = HttpRequest.new.get base + '/'
  body_ary = res["body"].split("\n")
  head = body_ary.first
  uid = body_ary[1].to_i
  gid = body_ary[2].to_i
  tail = body_ary.last

  #assert_equal "check", res["body"]
  t.assert_equal "__mod_mruby_head__", head
  t.assert_equal "__mod_mruby_tail__", tail
  t.assert_true uid >= 0
  t.assert_true gid >= 0
end

t.assert('mod_mruby', 'location /header') do
  res1 = HttpRequest.new.get base + '/header'
  res2 = HttpRequest.new.get base + '/header', nil, {"X-REQUEST-HEADER" => "hoge"}

  t.assert_equal "X-REQUEST-HEADER not found", res1["body"]
  t.assert_equal "nothing", res1["x-response-header"]
  t.assert_equal "X-REQUEST-HEADER found", res2["body"]
  t.assert_equal "hoge", res2["x-response-header"]
end

t.assert('mod_mruby', 'location /service-unavailable') do
  res = HttpRequest.new.get base + '/service-unavailable'
  t.assert_equal 503, res.code
end

t.assert('mod_mruby', 'location /document-root') do
  res = HttpRequest.new.get base + '/document-root'
  t.assert_equal "OK", res.body
end

t.assert('mod_mruby', 'location /request-body') do
  res = HttpRequest.new.post base + '/request-body', "request body"
  t.assert_equal "request body", res.body
end

t.assert('mod_mruby', 'location /note') do
  res = HttpRequest.new.post base + '/note'
  t.assert_equal "allow", res.body
end

t.assert('mod_mruby', 'location /basic/test') do
  token = base64 "user1:passwd"
  res = HttpRequest.new.post base + '/basic/test', nil, {"Authorization" => "Basic #{token}"}
  t.assert_equal "basic auth allowed", res.body
end

t.assert('mod_mruby', 'location /conn-ip') do
  res = HttpRequest.new.post base + '/conn-ip'
  t.assert_equal "server:#{server_addr},client:127.0.0.1", res.body
end

t.assert('mod_mruby', 'location /env?a=1') do
  res = HttpRequest.new.post base + '/env?a=1'
  t.assert_equal "QUERY_STRING:a=1", res.body
end

t.assert('mod_mruby', 'location /server-obj') do
  res = HttpRequest.new.post base + '/server-obj'
  t.assert_equal "Apache::Server", res.body
end

t.assert('mod_mruby', 'location /scoreboard-obj') do
  res = HttpRequest.new.post base + '/scoreboard-obj'
  t.assert_equal "Apache::Scoreboard", res.body
end

if Uname.sysname == 'Linux'
  t.assert('mod_mruby', 'location /tid') do
    res = HttpRequest.new.post base + '/tid'
    t.assert_true res.body.to_i > 0
  end
end

t.assert('mod_mruby', 'location /error-filter') do
  res = HttpRequest.new.post base + '/error-filter'
  t.assert_equal 503, res.code
end

t.assert('mod_mruby', 'location /body-filter') do
  res = HttpRequest.new.post base + '/body-filter'
  t.assert_equal "body-filter", res.body
end

t.assert('mod_mruby', 'location /server_class') do
  res = HttpRequest.new.get base + '/server_class'
  t.assert_equal "Apache", res.body
end

t.assert('mod_mruby', 'location /logger') do
  res = HttpRequest.new.get base + '/logger'
end

t.assert('mod_mruby', 'location /rack_base') do
  res = HttpRequest.new.get base + '/rack_base'
  t.assert_equal "rack body", res["body"]
  t.assert_equal "foo", res["x-hoge"]
  t.assert_equal 200, res.code
end

t.assert('mod_mruby', 'location /rack_base1') do
  res = HttpRequest.new.get base + '/rack_base1'
  t.assert_equal "rack body", res["body"]
  t.assert_equal "foo", res["x-hoge"]
  t.assert_equal "hoge", res["x-foo"]
  t.assert_equal 200, res.code
end

t.assert('mod_mruby', 'location /rack_base2') do
  res = HttpRequest.new.get base + '/rack_base2'
  t.assert_equal "rack body", res["body"]
  t.assert_equal "foo", res["x-hoge"]
  t.assert_equal "hoge", res["x-foo"]
  t.assert_equal 200, res.code
end

t.assert('mod_mruby', 'location /rack_base3') do
  res = HttpRequest.new.get base + '/rack_base3'
  t.assert_equal 404, res.code
end

t.assert('mod_mruby', 'location /rack_base_env') do
  res = HttpRequest.new.get base + '/rack_base_env?a=1&b=1', nil, {"Host" => "apache.example.com:38080", "x-hoge" => "foo"}
  body = JSON.parse res["body"]

  t.assert_equal "GET", body["REQUEST_METHOD"]
  t.assert_equal "", body["SCRIPT_NAME"]
  t.assert_equal "/rack_base_env", body["PATH_INFO"]
  t.assert_equal "/rack_base_env?a=1&b=1", body["REQUEST_URI"]
  t.assert_equal "a=1&b=1", body["QUERY_STRING"]
  t.assert_equal "apache.example.com", body["SERVER_NAME"]
  t.assert_equal "127.0.0.1", body["SERVER_ADDR"]
  t.assert_equal "38080", body["SERVER_PORT"]
  t.assert_equal "127.0.0.1", body["REMOTE_ADDR"]
  t.assert_equal "http", body["rack.url_scheme"]
  t.assert_false body["rack.multithread"]
  t.assert_true  body["rack.multiprocess"]
  t.assert_false body["rack.run_once"]
  t.assert_false body["rack.hijack?"]
  t.assert_equal "Apache", body["server.name"]
  t.assert_equal server_version, body["server.version"]
  t.assert_equal "*/*", body["HTTP_ACCEPT"]
  t.assert_equal "close", body["HTTP_CONNECTION"]
  t.assert_equal "apache.example.com:38080", body["HTTP_HOST"]
  t.assert_equal "foo", body["HTTP_X_HOGE"]
  t.assert_equal 200, res.code
end

t.assert('mod_mruby', 'location /rack_base_2phase') do
  res = HttpRequest.new.get base + '/rack_base_2phase', nil, {"auth-token" => "aaabbbccc"}
  t.assert_equal "OK", res["body"]
  t.assert_equal "127.0.0.1", res["x-client-ip"]
  t.assert_equal 200, res.code
end

t.assert('mod_mruby', 'location /rack_base_2phase') do
  res = HttpRequest.new.get base + '/rack_base_2phase', nil, {"auth-token" => "cccbbbaaa"}
  t.assert_equal 403, res.code
end

t.assert('mod_mruby', 'location /rack_base_push/index.txt') do
  res = HttpRequest.new.get base + '/rack_base_push/index.txt'
  t.assert_equal 200, res.code
  t.assert_equal "</index.js>; rel=preload", res["link"]
end

t.assert('mod_mruby', 'location /response_time') do
  res = HttpRequest.new.get base + '/response_time'
  t.assert_equal "2", res["body"]
end

t.report
