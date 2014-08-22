f = Apache::Filter.new
data = f.body
f.body = "body-filter"
