##
# mod_mruby test

# assert.rb global setteings
$ok_test = 0
$ko_test = 0
$kill_test = 0
$asserts  = []
$test_start = Time.now if Object.const_defined?(:Time)

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
  uid = body_ary[1].to_i
  gid = body_ary[2].to_i
  tail = body_ary.last

  #assert_equal "check", res["body"]
  assert_equal "__mod_mruby_head__", head
  assert_equal "__mod_mruby_tail__", tail
  assert_true uid >= 0
  assert_true gid >= 0
end

assert('mod_mruby', 'location /header') do
  res1 = HttpRequest.new.get base + '/header'
  res2 = HttpRequest.new.get base + '/header', nil, {"X-REQUEST-HEADER" => "hoge"}

  assert_equal "X-REQUEST-HEADER not found", res1["body"]
  assert_equal "nothing", res1["x-response-header"]
  assert_equal "X-REQUEST-HEADER found", res2["body"]
  assert_equal "hoge", res2["x-response-header"]
end

assert('mod_mruby', 'location /service-unavailable') do
  res = HttpRequest.new.get base + '/service-unavailable'
  assert_equal 503, res.code
end

assert('mod_mruby', 'location /document-root') do
  res = HttpRequest.new.get base + '/document-root'
  assert_equal "OK", res.body
end

assert('mod_mruby', 'location /request-body') do
  res = HttpRequest.new.post base + '/request-body', "request body"
  assert_equal "request body", res.body
end

assert('mod_mruby', 'location /note') do
  res = HttpRequest.new.post base + '/note'
  assert_equal "allow", res.body
end

assert('mod_mruby', 'location /basic/test') do
  token = base64 "user1:passwd"
  res = HttpRequest.new.post base + '/basic/test', nil, {"Authorization" => "Basic #{token}"}
  assert_equal "basic auth allowed", res.body
end

assert('mod_mruby', 'location /conn-ip') do
  res = HttpRequest.new.post base + '/conn-ip'
  assert_equal "server:#{server_addr},client:127.0.0.1", res.body
end

assert('mod_mruby', 'location /env?a=1') do
  res = HttpRequest.new.post base + '/env?a=1'
  assert_equal "QUERY_STRING:a=1", res.body
end

assert('mod_mruby', 'location /server-obj') do
  res = HttpRequest.new.post base + '/server-obj'
  assert_equal "Apache::Server", res.body
end

assert('mod_mruby', 'location /scoreboard-obj') do
  res = HttpRequest.new.post base + '/scoreboard-obj'
  assert_equal "Apache::Scoreboard", res.body
end

if Uname.sysname == 'Linux'
  assert('mod_mruby', 'location /tid') do
    res = HttpRequest.new.post base + '/tid'
    assert_true res.body.to_i > 0
  end
end

assert('mod_mruby', 'location /error-filter') do
  res = HttpRequest.new.post base + '/error-filter'
  assert_equal 503, res.code
end

assert('mod_mruby', 'location /body-filter') do
  res = HttpRequest.new.post base + '/body-filter'
  assert_equal "body-filter", res.body
end

# test report
report
if $ko_test > 0 or $kill_test > 0
    raise "mrbtest failed (KO:#{$ko_test}, Crash:#{$kill_test})"
end
