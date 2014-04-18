##
# ngx_mruby test

base = 'http://127.0.0.1'

assert('mod_mruby', 'location /hello-inline') do
  res = HttpRequest.new.get base + '/hello-inline'
  assert_equal 'hello', res["body"]
end

