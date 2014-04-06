if server_name == "NGINX"
  Server = Nginx
elsif server_name == "Apache"
  Server = Apache
end

r = Server::Request.new
s = Server::Server.new
c = Server::Connection.new
Server.echo "hostname: " + r.hostname
Server.echo "hello world"
Server.echo "documento_root: #{r.document_root}"
r.document_root = "/tmp"
Server.echo "documento_root: #{r.document_root}"
Server.echo "path: #{s.path}"
Server.echo "remote ip: #{c.remote_ip}"
Server.echo "remote port: #{c.remote_port}"
Server.echo "user_agent: #{r.headers_in.user_agent}"
Server.echo "local ip: #{c.local_ip}"
Server.echo "local port: #{c.local_port}"

r.headers_in.all.keys.each do |k|
  Server.echo "#{k}: #{r.headers_in[k]}"
end
if /Mac/ =~ r.headers_in.user_agent
  Server.echo "your pc is mac"
end
Apache.errlogger Apache::APLOG_ERR, "mod_mruby error!"
Apache.echo Apache.module_name
Apache.echo Apache.module_version
Apache.echo Apache.server_version
Apache.echo Apache.server_build
#r.error_log Apache::APLOG_NOTICE, "r.error_log"
#r.error_log_into "/tmp/error_log2"
Apache.echo r.the_request
Apache.echo r.the_request
r.the_request = r.the_request.gsub /GET/, 'POST'
Apache.echo r.the_request
Apache.echo r.protocol
r.protocol = r.protocol.gsub /1.1/, '1.0'
Apache.echo r.protocol
Apache.echo r.vlist_validator
Apache.echo r.user
Apache.echo r.ap_auth_type
Apache.echo r.unparsed_uri
Apache.echo r.uri
Apache.echo r.filename
Apache.echo r.canonical_filename
Apache.echo r.path_info
Apache.echo r.args
Apache.echo r.hostname
Apache.echo "----"
Apache.echo r.document_root
Apache.echo r.status_line
Apache.echo r.method
Apache.echo r.range
Apache.echo r.content_type
Apache.echo r.content_length
Apache.echo r.assbackwards
Apache.echo r.proxyreq
Apache.echo r.header_only
r.headers_out["X-HEAD-CHECK"] = r.header_only.to_s
Apache.echo r.proto_num
Apache.echo r.status
Apache.echo r.method_number
Apache.echo r.chunked
Apache.echo r.read_body
Apache.echo r.read_chunked
Apache.echo r.used_path_info
Apache.echo r.eos_sent
Apache.echo r.no_cache
Apache.echo r.no_local_copy
r.notes["test"] = "ok"
Apache.echo r.notes["test"]
r.headers_in.all.keys.each do |k|
  Apache.echo "#{k}: #{r.headers_in[k]}"
end
r.headers_out["X-HEAD-CHECK"] = r.header_only.to_s
r.headers_out["X-SEND-CHENKED-CHECK"] = r.chunked.to_s
r.headers_out["X-READ-CHENKED-CHECK"] = r.read_chunked.to_s
r.headers_out.all.keys.each do |k|
  Apache.echo "#{k}: #{r.headers_out[k]}"
end
Apache.echo "---- finfo ----"
Apache.echo r.finfo.permission
Apache.echo r.finfo.filetype
Apache.echo r.finfo.group
Apache.echo r.finfo.user
Apache.echo r.finfo.device
Apache.echo r.finfo.inode
Apache.echo r.finfo.nlink
Apache.echo r.finfo.size
Apache.echo r.finfo.csize
Apache.echo r.finfo.atime
Apache.echo r.finfo.ctime
Apache.echo r.finfo.mtime
Apache.echo "---- server ----"
Apache.echo s.error_fname
Apache.echo s.document_root
Apache.echo s.loglevel
Apache.echo s.hostname
Apache.echo s.path
Apache.echo s.pathlen
#Apache.echo s.scheme
Apache.echo s.defn_name
Apache.echo s.is_virtual
Apache.echo s.keep_alive_max
Apache.echo s.limit_req_line
Apache.echo s.limit_req_fieldsize
Apache.echo s.limit_req_fields
Apache.echo s.timeout
Apache.echo s.keep_alive_timeout
#Apache.echo s.port
Apache.echo s.defn_line_number
Apache.echo "---- conn ----"
Apache.echo c.remote_ip
Apache.echo c.remote_port
Apache.echo c.remote_host
Apache.echo c.remote_logname
Apache.echo c.local_ip
Apache.echo c.local_port
Apache.echo c.local_host
Apache.echo c.keepalives
Apache.echo c.data_in_input_filters
#Apache.echo "--- env ----"
#e = Apache::Env.new
#e.all.keys.each do |key|
#  Apache.echo "#{key}: #{e[key]}"
#end

#Apache.echo self.class.constants
#Apache.echo Object.methods
Apache.echo "--- init ----"
Apache.echo Userdata.new.hoge
Apache.echo Apache::M_GET.to_s
Apache.echo Apache::M_POST.to_s
