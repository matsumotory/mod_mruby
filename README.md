# Welcome to mod_mruby Pages  
[![Build Status](https://travis-ci.org/matsumoto-r/mod_mruby.svg?branch=master)](https://travis-ci.org/matsumoto-r/mod_mruby) [![wercker status](https://app.wercker.com/status/b1759ed571827bd44891a1f454ea9ec4/s "wercker status")](https://app.wercker.com/project/bykey/b1759ed571827bd44891a1f454ea9ec4)

[![ngx_mruby mod_mruby performance](https://github.com/matsumoto-r/mod_mruby/raw/master/images/performance_20140301.png)](http://blog.matsumoto-r.jp/?p=3974)

※ [hello world simple benchmark, see details of blog entry.](http://blog.matsumoto-r.jp/?p=3974)

## Documents
- [Install](https://github.com/matsumoto-r/mod_mruby/wiki/Install)
- [Test](https://github.com/matsumoto-r/mod_mruby/wiki/Test)
- [Directives](https://github.com/matsumoto-r/mod_mruby/wiki/Directives)
- [Class and Method](https://github.com/matsumoto-r/mod_mruby/wiki/Class-and-Method)
- [Use Case](https://github.com/matsumoto-r/mod_mruby/wiki/Use-Case)

## What's mod_mruby
__mod_murby is A Fast and Memory-Efficient Web Server Extension Mechanism Using Scripting Language mruby for Apache httpd.__

- Unified Ruby Code between Apache(mod_mruby), nginx(ngx_mruby) and other Web server software(plan) for Web server extensions.
- You can implement Apache modules by Ruby on Apache httpd.
- You can implement some Web server software extensions by same Ruby code (as possible)
- mod_mruby is to provide an alternative to mod_lua or [ngx_mruby of nginx](http://ngx.mruby.org/).
- Supported Apache Version: __2.2 2.4__
- Supported Apache MPM: __worker prefork event__
- Supported OS: __Linux FreeBSD Windows__ and so on.

```ruby
# Apache httpd.conf
# mrubyTranslateNameMiddle "/path/to/proxy.rb"
#

backends = [
  "http://192.168.0.101:8888/",
  "http://192.168.0.102:8888/",
  "http://192.168.0.103:8888/",
  "http://192.168.0.104:8888/",
]

# write balancing algorithm here.

r = Apache::Request.new()

r.handler  = "proxy-server"
r.proxyreq = Apache::PROXYREQ_REVERSE
r.filename = "proxy:" + backends[rand(backends.length)] + r.uri

Apache::return(Apache::OK)
```

- see [example](https://github.com/matsumoto-r/mod_mruby/tree/master/example)
- __Sample of Unified Ruby Code between Apache(mod_mruby) and nginx(ngx_mruby) for Web server extensions__
- You can implement some Web server software extensions by same Ruby code (as possible)

```ruby
# Unified Ruby Code between Apache(mod_mruby) and nginx(ngx_mruby)
# for Web server extensions.
#
# Apache httpd.conf by mod_mruby
#
# <Location /mruby>
#     mrubyHandlerMiddle "/path/to/unified_hello.rb"
# </Location>
#
# nginx ngxin.conf by ngx_mruby
#
# location /mruby {
#     mruby_content_handler "/path/to/unified_hello.rb";
# }
#

if server_name == "NGINX"
  Server = Nginx
elsif server_name == "Apache"
  Server = Apache
end

Server::rputs "Hello #{Server::module_name}/#{Server::module_version} world!"
# mod_mruby => "Hello mod_mruby/0.9.3 world!"
# ngx_mruby => "Hello ngx_mruby/0.0.1 world!"
```

## Abstract

As the increase of large-scale and complex Web services, not only a development of Web applications but also an implementation of Web server extensions is required in many cases. The Web server extensions were mainly implemented in C language because of fast and memory-efficient behavior, but extension methods using scripting language are proposed with consideration of maintainability and productivity. However, if the existing methods primarily intended to enhance not the implementation of Web applications but the implementation of internal processing of the Web server, the problem remains in terms of fast, memory-efficiency and safety. Therefore, we propose a fast and memory-efficient Web server extension mechanism using scripting language. We design the architecture that a server process create the region to save the state of the interpreter at the server process startup, and multiple scripts share the region in order to process fast when the script is called as internal processing from a Web server process. The server process free the global variables table, the exception flag and the byte-code which cause the increase of memory usage mainly, in order to reduce the memory usage and extend safely by preventing interference between each scripts because of sharing the region. We implement the mechanism that can extend the internal processing of Apache easily by Ruby scripts using Apache and embeddable scripting language mruby. It's called "mod_mruby".


# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php

