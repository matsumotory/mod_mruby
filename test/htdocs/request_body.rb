r = Apache::Request.new

if r.method_number == Apache::M_POST
  Apache.rputs r.body
else
  Apache.rputs "request body not found"
end
