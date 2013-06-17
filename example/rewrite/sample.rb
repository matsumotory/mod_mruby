r = Apache::Request.new

r.filename = "/var/www/html/redirect.html"

Apache::return Apache::OK
