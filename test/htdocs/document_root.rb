r = Apache::Request.new

r.document_root = "/tmp"

if r.document_root == "/tmp"
  Apache.rputs "OK"
else
  Apache.rputs "NG"
end
