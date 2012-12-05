# file limit control
#
# mrubyAccessCheckerMiddle /var/www/html/file_limit_inc_by_redis.rb
# mrubyLogTransactionMiddle /var/www/html/file_limit_dec_by_redis.rb

redis   = Apache::Redis.new("127.0.0.1", 6379)
req     = Apache::Request.new
limit   = 1

Apache.rputs "pahse 1"

fcounter = redis.get req.filename

if fcounter.nil? or fcounter.to_i < 0
  fcounter = 0.to_s
end

fcounter = (fcounter.to_i + 1).to_s
Apache.errlogger(4, "fcounter inc: #{req.filename}: #{fcounter.to_s}")

if fcounter.to_i > limit
  redis.set req.filename, fcounter
  Apache.return(503)
else
  redis.set req.filename, fcounter
end




