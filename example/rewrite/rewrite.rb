# RewriteEngine On
# RewriteRule ^/hoge/(.*) /fuga/$1
# RewriteRule ^/fuga/hage/(.*) /foo/$1

r = Apache::Request.new()
s = Apache::Server.new()

match_hoge = Regexp.new("^/hoge/(.*)").match(r.uri)

if match_hoge
  redirect   = "/fuga/" + match_hoge[1]
  match_hage = Regexp.new("^/fuga/hage/(.*)").match(redirect)

  if match_hage
    redirect   = "/foo/" + match_hage[1]
  end

  r.filename = s.document_root + redirect

  Apache.return(Apache::OK)
else
  Apache.return(Apache::DECLINED)
end
