class Worker
    def initialize(rate)
        @rate = rate
    end
    def check
        sb = Apache::Scoreboard.new()
        busyrate = sb.busy_worker / (sb.process_limit * sb.worker_limit)
        
        if busyrate * 100 > @rate
            true
        else
            false
        end
    end
end
 
r = Apache::Request.new()
s = Apache::Server.new()
w = Worker.new(50)
if w.check
    r.filename = s.document_root + "error/503.html"
    Apache::return(Apache::HTTP_SERVICE_UNAVAILABLE)
else
    Apache::return(Apache::OK)
end
