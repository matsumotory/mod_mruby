r = Apache::Request.new

orig = r.document_root
r.document_root = "/tmp"

if r.document_root == "/tmp"
  r.document_root = orig
  Apache.rputs "OK"
else
  r.document_root = orig
  Apache.rputs "NG"
end
