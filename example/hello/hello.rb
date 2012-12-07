#Apache.rputs("hello mod_mruby world!")
h = Apache::Hello.new

Apache.rputs(h.hello("matsumoto"))
