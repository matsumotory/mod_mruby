# LoadModule mruby_module modules/mod_mruby.so
# ExtendedStatus On
# mrubyAccessCheckerMiddle /var/www/html/traffic_rate_limit.rb

class Traffic
    def initialize(rate)
        @rate = rate
    end
    def kbcheck
        sb = Apache::Scoreboard.new()
        kbpersec = sb.total_kbyte / sb.uptime * 100
        Apache.errlogger(4, "kbpersec: " + kbpersec.to_s)
        if kbpersec > @rate
            true
        else
            false
        end
    end
end

# traffic kbyte / sec is 10000(10MB/sec)
t = Traffic.new(10000)
if t.kbcheck
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
else
    Apache::return(Apache::DECLINED)
end
