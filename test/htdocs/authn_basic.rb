user_list = {
  "user1" => "passwd",
}

anp = Apache::AuthnProvider.new

if user_list[anp.user] == anp.password
  Apache.return Apache::AuthnProvider::AUTH_GRANTED
else
  Apache.return Apache::AuthnProvider::AUTH_DENIED
end
