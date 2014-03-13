#
# mod_mruby API module
#
# LoadModule mruby_module modules/mod_mruby.so
# mrubyTranslateNameFirst "/tmp/api_request.rb"
#
# cp api_request.rb /tmp/.
# touch #{document_root}/dummy
#

module ModMruby
  def ModMruby.escape(str)
    reserved_str = [
      "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "n", "m", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
      "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
      "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
      "-", "_", ".", "~",
    ]
    tmp = ''
    str = str.to_s
    str.size.times do |idx|
      chr = str[idx]
      if reserved_str.include?(chr)
        tmp += chr
      end
    end
    tmp
  end
  class API
    def initialize(ip)
      @allow_ip = ip
      @a = Apache
      @r = @a::Request.new
      @s = @a::Server.new
    end
    def allow_access?
      c = Apache::Connection.new
      @a.errlogger 6, "allow ip: #{@allow_ip} <=> access ip: #{c.remote_ip}"
      if @allow_ip == c.remote_ip
        @a.errlogger 6, "api access OK: #{c.remote_ip}"
        true
      else
        @a.errlogger 6, "api access NG: #{c.remote_ip}. allow_ip is #{@allow_ip}"
        false
      end
    end
    def call
      param = @r.uri.split("/")
      
      el = {
        :id      => Random::rand(1000000),
        :key     => ModMruby.escape(param[1]),
        :type    => ModMruby.escape(param[2]),
        :class   => ModMruby.escape(param[3]),
        :method  => ModMruby.escape(param[4]),
        :val     => ModMruby.escape(param[5]),
      }

      @a.errlogger @a::APLOG_DEBUG, "=== [#{el[:id]}] api request [#{el[:class]}:#{el[:method]}]==="
      el.each_key do |key|
        @a.errlogger @a::APLOG_DEBUG, "#{key}: #{el[key]}"
      end
      @a.errlogger @a::APLOG_DEBUG, "=== [#{el[:id]}] api request [#{el[:class]}:#{el[:method]}]==="

      call_api(el)
    end
    def call_api(param)
      if param[:class] == "request"
        ModMruby::API::Request.new(param).send(param[:method])
      elsif param[:class] == "scoreboard"
        ModMruby::API::Scoreboard.new(param).send(param[:method])
      elsif param[:class] == "core"
        ModMruby::API::Core.new(param).send(param[:method])
      else
        @a.return Apache::DECLINED
      end
    end
    class Scoreboard
      def initialize(param)
        @a = Apache
        @r = Apache::Request.new
        @s = Apache::Server.new
        @sc = Apache::Scoreboard.new
        @param = param
        @r.content_type = "application/json"
        @r.filename = "#{@s.document_root}/dummy"
        @r.status = 200
        @a.return Apache::OK
      end
      def json_response(result); @a.rputs JSON::stringify(@param.merge result); end
      def busy_worker; result = {:result => {@param[:method] => @sc.busy_worker}}; json_response result; end
      def idle_worker; result = {:result => {@param[:method] => @sc.idle_worker}}; json_response result; end
      def total_kbyte; result = {:result => {@param[:method] => @sc.total_kbyte}}; json_response result; end
      def total_access; result = {:result => {@param[:method] => @sc.total_access}}; json_response result; end
      def counter;      result = {:result => {@param[:method] => @sc.counter}}; json_response result; end
      def all
        result = {:result => {
            :busy_worker   => @sc.busy_worker,
            :idle_worker   => @sc.idle_worker,
            :total_access  => @sc.total_access,
            :total_kbyte   => @sc.total_kbyte,
            :counter       => @sc.counter,
            :loadavg       => @sc.loadavg,
       }}
       json_response result
      end
    end
    class Request
      def initialize(param)
        @a = Apache
        @r = Apache::Request.new
        @s = Apache::Server.new
        @param = param
        @r.content_type = "application/json"
        @r.filename = "#{@s.document_root}/dummy"
        @r.status = 200
        @a.return Apache::OK
      end
      def uri; @a.rputs JSON::stringify(@param.merge({:result => {:uri => @r.uri}})); end
    end
    class Core
      def initialize(param)
        @a = Apache
        @r = Apache::Request.new
        @s = Apache::Server.new
        @param = param
        @r.content_type = "application/json"
        @r.filename = "#{@s.document_root}/dummy"
        @r.status = 200
        @a.return Apache::OK
      end
      def mod_mruby_version; @a.rputs JSON::stringify(@param.merge({:result => {@param[:method] => @a.mod_mruby_version}})); end
      def server_version; @a.rputs JSON::stringify(@param.merge({:result => {@param[:method] => @a.server_version}})); end
      def server_build; @a.rputs JSON::stringify(@param.merge({:result => {@param[:method] => @a.server_build}})); end
      def all
        @a.rputs JSON::stringify(@param.merge({:result => {
            :mod_mruby_version  => @a.mod_mruby_version,
            :server_version     => @a.server_version,
            :server_build       => @a.server_build,
          }}))
      end
    end
  end
end

allow_ip = '127.0.0.1'

api = ModMruby::API.new allow_ip
if api.allow_access?
  api.call
else
  Apache.return Apache::DECLINED
end
