# LoadModule mruby_module modules/mod_mruby.so
# ExtendedStatus On
# mrubyAccessCheckerMiddle /var/www/html/worker_rate_limit.rb

class Worker
    def initialize
        @sb = Apache::Scoreboard.new()
    end
    def busyrate
        @sb.busy_worker / (@sb.server_limit * @sb.thread_limit) * 100
    end
    def accessrate
        @sb.total_access / @sb.uptime
    end
end

# busy worker thread rate is 90 and access per sec is 1000
w = Worker.new()
if w.busyrate > 90
    Apache.errlogger(4, "busy worker rate: " + busyrate.to_s)
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
elsif w.accessrate > 10000
    Apache.errlogger(4, "access per sec: " + accesspsec.to_s)
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
else
    Apache::return(Apache::DECLINED)
end
