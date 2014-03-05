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
    end
    def code
    end
    def start
      Apache.echo "# mod_mruby Test Page"
      Apache.echo "__mod_mruby test start.__"
    end
    def finish
      Apache.echo "__Test Complete. Wellcome to mod_mruby world!!__"
      Apache.return(Apache::OK)
    end
    def core
      Apache.echo "## Apache Class Test"
      Apache.echo "- Server: #{server_name}"
      Apache.echo "- Apache version: #{Apache::server_version}"
      Apache.echo "- Apache build: #{Apache::server_build}"
      Apache.echo "- module name: #{Apache::module_name}"
      Apache.echo "- module version: #{Apache::module_version}"
      Apache.syslogger "warn", "mod_mruby warning syslogger test."
      Apache.echo "- syslogger function: OK"
      Apache.errlogger 4, "mod_mruby warning errlogger test."
      Apache.echo "- errlogger function: OK"
      Apache.echo "- OK status = #{Apache::OK.to_s}"
      Apache.echo "- DECLINED status = #{Apache::DECLINED.to_s}"
      Apache.echo "- HTTP_OK status = #{Apache::HTTP_OK.to_s}"
      Apache.echo "- HTTP_SERVICE_UNAVAILABLE status = #{Apache::HTTP_SERVICE_UNAVAILABLE.to_s}"
    end
    def request
      Apache.echo "## Request Class Test"
      r = Apache::Request.new()
      r.content_type = "text/plain"
      Apache.echo "- filename = #{r.filename}"
      Apache.echo "- uri = #{r.uri}"
      Apache.echo "- ---- request_rec changed ----"
      r.filename = "/var/www/html/index.html"
      r.uri = "/index.html"
      Apache.echo "- filename = #{r.filename}"
      Apache.echo "- uri = #{r.uri}"
      p_docroot = r.document_root
      Apache.echo "r.document_root: #{r.document_root}"
      r.document_root = "/tmp"
      Apache.echo "r.document_root: #{r.document_root}"
      r.document_root = p_docroot
    end
    def server
      Apache.echo "## Server Class Test"
      s = Apache::Server.new()
      Apache.echo "- document_root = #{s.document_root}"
      Apache.echo "- error_fname = #{s.error_fname}"
      Apache.echo "- hostname = #{s.hostname}"
      Apache.echo "- timeout = #{s.timeout.to_s}"
    end
    def connection
      Apache.echo "## Connection Class Test"
      c = Apache::Connection.new()
      Apache.echo "- remote_ip #{c.remote_ip}"
      Apache.echo "- local_ip #{c.local_ip}"
      Apache.echo "- keepalives = #{c.keepalives.to_s}"
    end
    def note
      Apache.echo "##Notes Class Test"
      n = Apache::Request.new.notes
      n["memo"] = "hello"
      Apache.echo "- memo = #{n['memo']}"
    end
    def env
      Apache.echo "## Env Class Test"
      e = Apache::Env.new()
      e["AUTHOR"] = "MATSUMOTO Ryosuke"
      e["AUTHOR"] = "matsumoto_r"
      Apache.echo "- AUTHOR = #{e["AUTHOR"]}"
      Apache.echo "- SERVER_SOFTWARE = #{e['SERVER_SOFTWARE'].nil? ? 'nil' : e['SERVER_SOFTWARE']}"
      e["SERVER_SOFTWARE"] = "A p a c h e"
      eall = e.all
      Apache.echo "- env hash size = #{eall.size.to_s}"
      eall.each_key do |key|
          Apache.echo "- env key = #{key} val = #{eall[key].to_s}"
      end
    end
    def headers_in
      Apache.echo "## Headers_in Class Test"
      hi = Apache::Request.new.headers_in
      Apache.echo "- Accept-Encoding = #{hi['Accept-Encoding'].nil? ? 'nil' : hi['Accept-Encoding']}"
      hi["Accept-Encoding"] = "gzip"
      Apache.echo "- Accept-Encoding = #{hi["Accept-Encoding"]}"
      hiall = hi.all
      Apache.echo "- headers_in hash size = #{hiall.size.to_s}"
      hiall.each_key do |key|
        Apache.echo "- headers_in key = #{key} val = #{hiall[key]}"
      end
    end
    def headers_out
      Apache.echo "## Headers_out Class Test"
      ho = Apache::Request.new.headers_out
      hoall = ho.all
      Apache.echo "- headers_out hash size = #{hoall.size.to_s}"
      hoall.each_key do |key|
        Apache.echo "- headers_in key = #{key}  val = #{hoall[key]}"
      end
    end
    def scoreboard
      Apache.echo "## Scoreboard Class Test"
      wc = Apache::Scoreboard.new()
      Apache.echo "- child pid = #{wc.pid.to_s}"
      Apache.echo "- thread limit    = #{wc.thread_limit.to_s}"
      Apache.echo "- server limit    = #{wc.server_limit.to_s}"
      Apache.echo "__Cannot get below values of scoreboard because of ExtendedStatus Off__" if wc.restart_time == 0
      Apache.echo "- cpu load        = #{wc.cpu_load.to_s}"
      Apache.echo "- load avereage   = #{wc.loadavg.to_s}"
      Apache.echo "- total kbyte     = #{wc.total_kbyte.to_s}"
      Apache.echo "- total access    = #{wc.total_access.to_s}"
      Apache.echo "- restart time    = #{wc.restart_time.to_s}"
      Apache.echo "- idle worker     = #{wc.idle_worker.to_s}"
      Apache.echo "- busy worker     = #{wc.busy_worker.to_s}"
      Apache.echo "- uptime          = #{wc.uptime.to_s}"
      Apache.echo "- access counter  = #{wc.access_counter(wc.pid).to_s}"
      Apache.echo "- scoreboard status hash size = #{wc.status.size.to_s}"
      wc.status.each_key do |key|
          Apache.echo "- scoreboard key = #{key} val = #{wc.status[key]}"
      end
      Apache.echo "- scoreboard counter hash size = #{wc.counter.size.to_s}"
      wc.counter.each_key do |key|
          Apache.echo "- scoreboard key = #{key} val = #{wc.counter[key].to_s}"
      end
    end
    def finfo
      Apache::echo "## Finfo Class Test"
      f = Apache::Request.new.finfo
      Apache.echo "- permission = #{f.permission.to_s}"
      Apache.echo "- filetype regular file = #{f.filetype == Apache::Finfo::APR_REG}"
      Apache.echo "- user = #{f.user.to_s}"
      Apache.echo "- group = #{f.group.to_s}"
      Apache.echo "- device = #{f.device.to_s}"
      Apache.echo "- inode = #{f.inode.to_s}"
      Apache.echo "- nlink = #{f.nlink.to_s}"
      Apache.echo "- size = #{f.size.to_s}"
      Apache.echo "- csize = #{f.csize.to_s}"
      Apache.echo "- atime = #{f.atime.to_s}"
      Apache.echo "- mtime = #{f.mtime.to_s}"
      Apache.echo "- ctime = #{f.ctime.to_s}"
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
