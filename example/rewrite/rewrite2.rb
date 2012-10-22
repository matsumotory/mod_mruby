# RewriteEngine on
# RewriteRule ^/hoge/$ /fuga/

r = Apache::Request.new()
s = Apache::Server.new()

match_hoge = Regexp.new("^/hoge/(.*)").match(r.uri)

if match_hoge
  redirect   = "/fuga/" + match_hoge[1]
  r.filename = s.document_root + redirect

  Apache.return(Apache::OK)
else
  Apache.return(Apache::DECLINED)
end
