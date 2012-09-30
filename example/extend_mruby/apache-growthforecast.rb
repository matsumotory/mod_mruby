##
# SimpleURI

class SimpleURI
  PARAMS_ORDER = [:scheme, :userinfo, :host, :port, :registry, :path, :opaque, :query, :fragment]

  def initialize(*args)
    args = SimpleURI.split(args.first)  if args.length == 1
    @uri = {}
    args.each_index{|idx|
      @uri[PARAMS_ORDER[idx]] = args[idx]
    }
    validate
    self
  end

  def validate
    # TODO
  end

  def self.parse(str)
    args = SimpleURI.split(str)
    SimpleURI.new(*args)
  end

  def self.split(str)
    raise InvalidURIError unless str
    uri = {}

    pattern_scheme_host_path = "\\A(https?|ftp)\://([^\/]+)(\/.*)?\\Z"
    m = Regexp.new(pattern_scheme_host_path).match(str)
    raise InvalidURIError unless m

    uri[:scheme] = m[1]

    if m[2].include?('@')
      uri[:userinfo], uri[:host] = m[2].split('@')
    else
      uri[:host] = m[2]
    end

    if uri[:host].include?(':')
      uri[:host], uri[:port] = uri[:host].split(':')
    end

    if m[3] and m[3].include?('?')
      uri[:path], uri[:query] = m[3].split('?')
    else
      uri[:path] = m[3]
    end

    uri[:path] = "/"  unless uri[:path]

    PARAMS_ORDER.map{|key| uri[key] }
  end

  def [](key); @uri[key.to_sym]; end
  def []=(key, value);  @uri[key.to_sym] = value; end

  def request_uri
    if @uri[:query]
      @uri[:path] + '?' + @uri[:query]
    else
      @uri[:path]
    end
  end

  def to_s
    str = scheme + "://"
    str += userinfo + "@"  if userinfo
    str += host
    str += ":" + port  if port
    str += path if path
    str += "?" + query  if query
    str
  end

  def scheme;   @uri[:scheme];   end
  def userinfo; @uri[:userinfo]; end
  def host;     @uri[:host];     end
  def port;     @uri[:port];     end
  def path;     @uri[:path];     end
  def query;    @uri[:query];    end
  def scheme=(s);   @uri[:scheme]   = s; end
  def userinfo=(s); @uri[:userinfo] = s; end
  def host=(s);     @uri[:host]     = s; end
  def port=(s);     @uri[:port]     = s; end
  def path=(s);     @uri[:path]     = s; end
  def query=(s);    @uri[:query]    = s; end

  class Error < StandardError; end
  class InvalidURIError < SimpleURI::Error; end

  def self.escape(str, unsafe = nil)
    tmp = ''
    str = str.to_s
    str.size.times do |idx|
      chr = str[idx]
      if unsafe.nil? or unsafe.match(chr).nil?
        tmp += chr
      else
        tmp += "%" + chr.unpack("H*").first.upcase
      end
    end
    tmp
  end
end


##
# Simple Http

class SimpleHttp
  DEFAULTPORT = 80
  HTTP_VERSION = "HTTP/1.0"
  DEFAULT_ACCEPT = "*/*"
  SEP = "\r\n"

  def initialize(address, port = DEFAULTPORT)
    @socket
    @uri = {}
    @uri[:address] = address
    @uri[:port] = port ? port.to_i : DEFAULTPORT
    self
  end

  def address; @uri[:address]; end
  def port; @uri[:port]; end

  def get(path = "/", request = nil)
    request("GET", path, request)
  end

  def post(path = "/", request = nil)
    request("POST", path, request)
  end

  # private
  def request(method, path, req)
    @uri[:path] = path
    if @uri[:path].nil?
      @uri[:path] = "/"
    elsif @uri[:path][0] != "/"
      @uri[:path] = "/" + @uri[:path]
    end
    request_header = create_request_header(method.upcase.to_s, req)
    response_text = send_request(request_header)
    SimpleHttpResponse.new(response_text)
  end

  def send_request(request_header)
    @socket = TCPSocket.new(@uri[:address], @uri[:port])
    @socket.write(request_header)
    response_text = ""
    while (t = @socket.read(1024))
      response_text += t
    end
    @socket.close
    response_text
  end

  def create_request_header(method, req)
    req = {}  unless req
    str = ""
    body   = ""
    str += sprintf("%s %s %s", method, @uri[:path], HTTP_VERSION) + SEP
    header = {}
    req.each do |key,value|
      header[key.capitalize] = value
    end
    header["Host"] = @uri[:address]  unless header.keys.include?("Host")
    header["Accept"] = DEFAULT_ACCEPT  unless header.keys.include?("Accept")
    header["Connection"] = "close"
    if header["Body"]
      body = header["Body"]
      header.delete("Body")
    end
    if method == "POST" && (not header.keys.include?("content-length".capitalize))
        header["Content-Length"] = (body || '').length
    end
    header.keys.sort.each do |key|
      str += sprintf("%s: %s", key, header[key]) + SEP
    end
    str + SEP + body
  end

  class SimpleHttpResponse
    SEP = SimpleHttp::SEP
    def initialize(response_text)
      @response = {}
      if response_text.include?(SEP + SEP)
        @response["header"], @response["body"] = response_text.split(SEP + SEP)
      else
        @response["header"] = response_text
      end
      parse_header
      self
    end

    def [](key); @response[key]; end
    def []=(key, value);  @response[key] = value; end

    def header; @response['header']; end
    def body; @response['body']; end
    def status; @response['status']; end
    def code; @response['code']; end
    def date; @response['date']; end
    def content_type; @response['content-type']; end
    def content_length; @response['content-length']; end

    def each(&block)
      if block
        @response.each do |k,v| block.call(k,v) end
      end
    end
    def each_name(&block)
      if block
        @response.each do |k,v| block.call(k) end
      end
    end

    # private
    def parse_header
      return unless @response["header"]
      h = @response["header"].split(SEP)
      if h[0].include?("HTTP/1")
        @response["status"] = h[0].split(" ", 2).last
        @response["code"]   = h[0].split(" ", 3)[1].to_i
      end
      h.each do |line|
        if line.include?(": ")
          k,v = line.split(": ")
          @response[k.downcase] = v
        end
      end
    end
  end
end

# HttpRequest Class

class HttpRequest

  def get(url, headers = {})
    request("GET", url, nil, headers)
  end

  def head(url, headers = {})
    request("HEAD", url, nil, headers)
  end

  def post(url, body = nil, headers = {})
    request("POST", url, body, headers)
  end

  def put(url, body = nil, headers = {})
    request("PUT", url, body, headers)
  end

  def delete(url, headers = {})
    request("DELETE", url, nil, headers)
  end

  # private
  def request(method, url, body, headers)
    url = SimpleURI.parse(url)
    request = create_http_request(method, body, headers)
    host = url.host.to_sym.to_s
    p method, url.request_uri, request
    SimpleHttp.new(host, url.port).request(method, url.request_uri, request)
  end

  def encode_parameters(params, delimiter = '&', quote = nil)
    if params.is_a?(Hash)
      params = params.map do |key, value|
        sprintf("%s=%s%s%s", key, quote, value, quote)
      end
    else
      params = params.map { |value| escape(value) }
    end
    delimiter ? params.join(delimiter) : params
  end

  def create_http_request(method, body, headers)
    method = method.upcase.to_s
    request = {}
    request['User-Agent'] = "mod_mruby_ua"
    if method == "POST" || method == "PUT"
      request["body"] = body.is_a?(Hash) ? encode_parameters(body) : body.to_s
      request["Content-Type"] = 'application/x-www-form-urlencoded'
      request["Content-Length"] = (request["body"] || '').length
    end
    request
  end
end

host    = ""
port    = "5125"

service = "apache"
section = "scoreboad"
gragh   = "worker"

http       = HttpRequest.new()
score      = Apache::Scoreboard.new()
headers_in = Apache::Headers_in.new()
url        = sprintf("http://%s:%s/api/%s/%s/%s", host, port, service, section, gragh)

if headers_in["X-PushStatus"]
  Apache.rputs("X-PushStatus = " + headers_in["X-PushStatus"].to_s + "<br>")
  response = http.post(url, {
    :number => score.busy_worker,
    :color  => "#333399"
  })
  Apache.rputs("response = " + response.body)
end
