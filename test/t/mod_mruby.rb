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

