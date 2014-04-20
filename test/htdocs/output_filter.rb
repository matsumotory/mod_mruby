f = Apache::Filter.new

data = f.flatten
f.cleanup

f.insert_tail data
f.insert_tail "__mod_mruby_tail__\n"
f.insert_head "__mod_mruby_head__\n"
f.insert_eos
