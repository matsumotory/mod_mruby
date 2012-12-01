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

user_list = {
  "bilbo" => "foo",
  "frodo" => "bar",
  "samwise" => "baz",
  "aragorn" => "qux",
  "legolas" => "quux",
  "gimli" => "corge",
}

anp = Apache::AuthnProvider.new

if user_list[anp.user] == anp.password
  Apache.return(Apache::AuthnProvider::AUTH_GRANTED)
else
  Apache.return(Apache::AuthnProvider::AUTH_DENIED)
end
