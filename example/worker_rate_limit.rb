# LoadModule mruby_module modules/mod_mruby.so
# ExtendedStatus On
# mrubyAccessCheckerMiddle /var/www/html/worker_rate_limit.rb

class Worker
    def initialize(rate)
        @rate = rate
    end
    def check
        sb = Apache::Scoreboard.new()
        busyrate = sb.busy_worker / (sb.server_limit * sb.thread_limit) * 100
        if busyrate > @rate
            true
        else
            false
        end
    end
end

# busy worker thread rate is 90
w = Worker.new(90)
if w.check
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
else
    Apache::return(Apache::OK)
end
