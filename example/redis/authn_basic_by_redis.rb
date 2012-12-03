# Basic Authentication Sample
#
# Apache configuration
#
# <Location /basic/>
#   AuthType basic
#   AuthName "Message for clients"
#   AuthBasicProvider mruby
#   mrubyAuthnCheckPassword /path/to/authn_basic.rb
#   require valid-user
# </Location>
#

a       = Apache

host    = "127.0.0.1"
port    = 6379

anp     = Apache::AuthnProvider.new
redis   = Apache::Redis.new(host, port)

if redis.get(anp.user) == anp.password
  Apache.return(Apache::AuthnProvider::AUTH_GRANTED)
else
  Apache.return(Apache::AuthnProvider::AUTH_DENIED)
end
