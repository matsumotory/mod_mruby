# mrubyFixupsMiddle "/usr/local/apache/hooks/test.rb"
$hoge = 1
Apache::rputs global_variables.to_s
Apache::rputs "hoge"
Apache::rputs Apache::global_remove(:$hoge).to_s
