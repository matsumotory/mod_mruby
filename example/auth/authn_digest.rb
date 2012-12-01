# Digest Authentication Sample
#
# Note:
#   This sample requires iij/mruby to use Digest::MD5
#
# Apache configuration
#
# <Location /digest/>
#   AuthType digest
#   AuthName "hobbits"
#   AuthBasicProvider mruby
#   mrubyAuthnGetRealmHash /path/to/authn_digest.rb
#   require valid-user
# </Location>
#

realm_user_list = {
  "hobbits" => {
    "bilbo" => "foo",
    "frodo" => "bar",
    "samwise" => "baz",
  },
  "humans" => {
    "aragorn" => "qux",
  },
  "elves" => {
    "legolas" => "quux",
  },
  "dwarves" => {
    "gimli" => "corge",
  },
}

anp = Apache::AuthnProvider.new

user_list = realm_user_list[anp.realm]
if user_list.nil?
  Apache.return(Apache::AuthnProvider::AUTH_USER_NOT_FOUND)
else
  password = user_list[anp.user]
  if password.nil?
    Apache.return(Apache::AuthnProvider::AUTH_USER_NOT_FOUND)
  else
    anp.rethash = Digest::MD5.hexdigest([anp.user, anp.realm, password].join(":"))
    Apache.return(Apache::AuthnProvider::AUTH_USER_FOUND)
  end
end
