$hoge = 1

Apache::rputs global_variables.to_s
Apache::rputs Apache::remove_global_variable(:$hoge).to_s
