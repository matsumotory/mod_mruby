note = Apache::Notes.new

target_ip = Apache::Connection.new.remote_ip

note[target_ip] = "allow"
