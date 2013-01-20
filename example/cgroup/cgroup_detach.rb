r = Apache::Request.new
 
if r.filename == "/var/www/html/while.cgi"
  c = Cgroup::CPU.new("apache/mod_mruby_group")
  if c.exist?
    c.delete
  end
end
