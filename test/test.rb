#
# mod_mruby test sample script
#
# vi /etc/httpd/conf.d/mruby.conf
# LoadModule mruby_module /usr/lib/httpd/modules/mod_mruby.so
# AddHandler mruby-script .rb
#
# access to http://example.com/test.rb
#

module ModMruby
  class Test
    def initialize
      @md = Discount.new("https://gist.github.com/andyferra/2554919/raw/2e66cabdafe1c9a7f354aa2ebf5bc38265e638e5/github.css", "mod_mruby test page")
    end
    def code
    end
    def start
      Apache.rputs @md.header
      Apache.rputs "# mod_mruby Test Page".to_html
      Apache.rputs "__mod_mruby test start.__".to_html
    end
    def finish
      Apache.rputs "__Test Complete. Wellcome to mod_mruby world!!__".to_html
      Apache.rputs @md.footer
      Apache.return(Apache::OK)
    end
    def core
      Apache.rputs "## Apache Class Test".to_html
      Apache.rputs "- Apache version: #{Apache::server_version}".to_html
      Apache.rputs "- Apache build: #{Apache::server_build}".to_html
      Apache.rputs "- mod_mruby version: #{Apache::mod_mruby_version}".to_html
      Apache.syslogger "warn", "mod_mruby warning syslogger test."
      Apache.rputs "- syslogger function: OK".to_html
      Apache.errlogger 4, "mod_mruby warning errlogger test."
      Apache.rputs "- errlogger function: OK".to_html
      Apache.rputs "- OK status = #{Apache::OK.to_s}".to_html
      Apache.rputs "- DECLINED status = #{Apache::DECLINED.to_s}".to_html
      Apache.rputs "- HTTP_OK status = #{Apache::HTTP_OK.to_s}".to_html
      Apache.rputs "- HTTP_SERVICE_UNAVAILABLE status = #{Apache::HTTP_SERVICE_UNAVAILABLE.to_s}".to_html
    end
    def request
      Apache.rputs "## Request Class Test".to_html
      r = Apache::Request.new()
      r.content_type = "text/html"
      Apache.rputs "- filename = #{r.filename}".to_html
      Apache.rputs "- uri = #{r.uri}".to_html
      Apache.rputs "- ---- request_rec changed ----".to_html
      r.filename = "/var/www/html/index.html"
      r.uri = "/index.html"
      Apache.rputs "- filename = #{r.filename}".to_html
      Apache.rputs "- uri = #{r.uri}".to_html
    end
    def server
      Apache.rputs "## Server Class Test".to_html
      s = Apache::Server.new()
      Apache.rputs "- document_root = #{s.document_root}".to_html
      Apache.rputs "- error_fname = #{s.error_fname}".to_html
      Apache.rputs "- hostname = #{s.hostname}".to_html
      Apache.rputs "- timeout = #{s.timeout.to_s}".to_html
    end
    def connection
      Apache.rputs "## Connection Class Test".to_html
      c = Apache::Connection.new()
      Apache.rputs "- remote_ip #{c.remote_ip}".to_html
      Apache.rputs "- local_ip #{c.local_ip}".to_html
      Apache.rputs "- keepalives = #{c.keepalives.to_s}".to_html
    end
    def note
      Apache.rputs "##Notes Class Test".to_html
      n = Apache::Request.new.notes
      n["memo"] = "hello"
      Apache.rputs "- memo = #{n['memo']}".to_html
    end
    def env
      Apache.rputs "## Env Class Test".to_html
      e = Apache::Env.new()
      e["AUTHOR"] = "MATSUMOTO Ryosuke"
      e["AUTHOR"] = "matsumoto_r"
      Apache.rputs "- AUTHOR = #{e["AUTHOR"]}".to_html
      Apache.rputs "- SERVER_SOFTWARE = #{e['SERVER_SOFTWARE'].nil? ? 'nil' : e['SERVER_SOFTWARE']}".to_html
      e["SERVER_SOFTWARE"] = "A p a c h e"
      eall = e.env_hash
      Apache.rputs "- env hash size = #{eall.size.to_s}".to_html
      eall.each_key do |key|
          Apache.rputs "- env key = #{key} val = #{eall[key].to_s}".to_html
      end
    end
    def headers_in
      Apache.rputs "## Headers_in Class Test".to_html
      hi = Apache::Request.new.headers_in
      Apache.rputs "- Accept-Encoding = #{hi['Accept-Encoding'].nil? ? 'nil' : hi['Accept-Encoding']}".to_html
      hi["Accept-Encoding"] = "gzip"
      Apache.rputs "- Accept-Encoding = #{hi["Accept-Encoding"]}".to_html
      hiall = hi.all
      Apache.rputs "- headers_in hash size = #{hiall.size.to_s}".to_html
      hiall.each_key do |key|
        Apache.rputs "- headers_in key = #{key} val = #{hiall[key]}".to_html
      end
    end
    def headers_out
      Apache.rputs "## Headers_out Class Test".to_html
      ho = Apache::Request.new.headers_out
      hoall = ho.all
      Apache.rputs "- headers_out hash size = #{hoall.size.to_s}".to_html
      hoall.each_key do |key|
        Apache.rputs "- headers_in key = #{key}  val = #{hoall[key]}".to_html
      end
    end
    def scoreboard
      Apache.rputs "## Scoreboard Class Test".to_html
      wc = Apache::Scoreboard.new()
      Apache.rputs "- child pid = #{wc.pid.to_s}".to_html
      Apache.rputs "- thread limit    = #{wc.thread_limit.to_s}".to_html
      Apache.rputs "- server limit    = #{wc.server_limit.to_s}".to_html
      Apache.rputs "__Cannot get below values of scoreboard because of ExtendedStatus Off__".to_html if wc.restart_time == 0
      Apache.rputs "- cpu load        = #{wc.cpu_load.to_s}".to_html
      Apache.rputs "- load avereage   = #{wc.loadavg.to_s}".to_html
      Apache.rputs "- total kbyte     = #{wc.total_kbyte.to_s}".to_html
      Apache.rputs "- total access    = #{wc.total_access.to_s}".to_html
      Apache.rputs "- restart time    = #{wc.restart_time.to_s}".to_html
      Apache.rputs "- idle worker     = #{wc.idle_worker.to_s}".to_html
      Apache.rputs "- busy worker     = #{wc.busy_worker.to_s}".to_html
      Apache.rputs "- uptime          = #{wc.uptime.to_s}".to_html
      Apache.rputs "- access counter  = #{wc.access_counter(wc.pid).to_s}".to_html
      Apache.rputs "- scoreboard status hash size = #{wc.status.size.to_s}".to_html
      wc.status.each_key do |key|
          Apache.rputs "- scoreboard key = #{key} val = #{wc.status[key]}".to_html
      end
      Apache.rputs "- scoreboard counter hash size = #{wc.counter.size.to_s}".to_html
      wc.counter.each_key do |key|
          Apache.rputs "- scoreboard key = #{key} val = #{wc.counter[key].to_s}".to_html
      end
    end
    def finfo
      Apache::rputs "## Finfo Class Test".to_html
      f = Apache::Request.new.finfo
      Apache.rputs "- permission = #{f.permission.to_s}".to_html
      Apache.rputs "- filetype regular file = #{f.filetype == Apache::Finfo::APR_REG}".to_html
      Apache.rputs "- user = #{f.user.to_s}".to_html
      Apache.rputs "- group = #{f.group.to_s}".to_html
      Apache.rputs "- device = #{f.device.to_s}".to_html
      Apache.rputs "- inode = #{f.inode.to_s}".to_html
      Apache.rputs "- nlink = #{f.nlink.to_s}".to_html
      Apache.rputs "- size = #{f.size.to_s}".to_html
      Apache.rputs "- csize = #{f.csize.to_s}".to_html
      Apache.rputs "- atime = #{f.atime.to_s}".to_html
      Apache.rputs "- mtime = #{f.mtime.to_s}".to_html
      Apache.rputs "- ctime = #{f.ctime.to_s}".to_html
    end
  end
end

t = ModMruby::Test.new
t.start
t.code
t.core
t.request
t.server
t.connection
t.note
t.env
t.headers_in
t.headers_out
t.scoreboard
t.finfo
t.finish
Apache.errlogger 4, "arena count = #{Apache.count_arena.to_s}"
