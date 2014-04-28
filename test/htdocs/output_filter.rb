f = Apache::Filter.new

data = f.flatten
f.cleanup

f.insert_tail data
f.insert_tail "\n__mod_mruby_tail__"
f.insert_head "__mod_mruby_head__\n"
f.insert_eos
