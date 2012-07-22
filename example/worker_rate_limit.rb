# LoadModule mruby_module modules/mod_mruby.so
# ExtendedStatus On
# mrubyAccessCheckerMiddle /var/www/html/worker_rate_limit.rb

class Worker
    def initialize(rate, count)
        @sb = Apache::Scoreboard.new()
        @myrate = rate
        @mycount = count
    end
    def busycheck
        busyrate = @sb.busy_worker / (@sb.server_limit * @sb.thread_limit) * 100
        Apache.errlogger(4, "busy worker rate: " + busyrate.to_s)
        if busyrate > @myrate
            true
        else
            false
        end
    end
    def countcheck
        accesspsec = @sb.total_access / @sb.uptime
        Apache.errlogger(4, "access per sec: " + accesspsec.to_s)
        if accesspsec > @mycount
            true
        else
            false
        end
    end
end

# busy worker thread rate is 90 and access per sec is 1000
w = Worker.new(90, 1000)
if w.busycheck
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
elsif w.countcheck
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
else
    Apache::return(Apache::DECLINED)
end
