# file limit control
#
# mrubyAccessCheckerMiddle /var/www/html/file_limit_inc_by_redis.rb
# mrubyLogTransactionMiddle /var/www/html/file_limit_dec_by_redis.rb

redis   = Apache::Redis.new("127.0.0.1", 6379)
req     = Apache::Request.new

fcounter = redis.get req.filename

if fcounter.nil? or fcounter.to_i <= 0
  Apache.errlogger(4, "unexpect error: fcounter is nil or <= 0 at dec phase")
  redis.set req.filename, fcounter
else
  fcounter = (fcounter.to_i - 1).to_s
  Apache.errlogger(4, "fcounter dec: #{req.filename}: #{fcounter.to_s}")
  redis.set req.filename, fcounter
end
