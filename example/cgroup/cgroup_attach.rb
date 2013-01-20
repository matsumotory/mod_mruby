r = Apache::Request.new
 
if r.filename == "/var/www/html/while.cgi"
  c = Cgroup::CPU.new("apache/mod_mruby_group")
  unless c.exist?
    c.create
  end
  c.cfs_quota_us = 10000
  c.attach
end
