#
# httpd.conf
#
# SetOutputFilter mruby
# mrubyOutputFilter /usr/local/apache/htdocs/filter.rb
#
f = Apache::Filter.new

# read all body data
data = f.flatten

# cleanup old brigade
f.cleanup

# create new data
# create string
f.puts "foofoo-puts"

# modify body data
f.insert_tail data.upcase

# insert string to tail
f.insert_tail "hogehoge-tail"

# insert string to head
f.insert_head "fugafuga-head"

# insert End of String
f.insert_eos

# $ curl http://127.0.0.1/index.html
# fugafuga-headfoofoo-puts<HTML><BODY><H1>IT WORKS!</H1></BODY></HTML>
# hogehoge-tail
