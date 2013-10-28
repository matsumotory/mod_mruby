# Welcome to mod_mruby Pages
## What's mod_mruby
__mod_murby is A Fast and Memory-Efficient Web Server Extension Mechanism Using Scripting Language mruby for Apache httpd.__

- Unified Ruby Code between Apache(mod_mruby), nginx(ngx_mruby) and other Web server software(plan) for Web server extensions.
- You can implement Apache modules by Ruby on Apache httpd.
- You can implement some Web server software extensions by same Ruby code (as possible) 
- mod_mruby is to provide an alternative to mod_lua or [ngx_mruby of nginx](http://matsumoto-r.github.io/ngx_mruby/).
- Supported Apache Version: __2.0 2.2 2.4 2.5__
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

## How to use
### 1. Download
    git clone git://github.com/matsumoto-r/mod_mruby.git

### 2. Auto Build
    cd mod_mruby
    sh build.sh

### 3. Test Settings
* Add to httpd.conf

    ```apache
    LoadModule mruby_module modules/mod_mruby.so
    AddHandler mruby-script .rb  
    ```
or hook from a handler phase
    ```apache
    LoadModule mruby_module modules/mod_mruby.so
    <Location /mruby-test>
        mrubyHandlerMiddle /path/to/test.rb
    </Location>
    ```
or cache enabled
    ```apache
    LoadModule mruby_module modules/mod_mruby.so
    <Location /mruby-test>
        mrubyHandlerMiddle /path/to/test.rb cache
    </Location>
    ```
        
* test.rb copy   

        cp -p test/test.rb $(APACHE_DOCMENT_ROOT)/.

### 4. Apache Restart
    service httpd restart

### 5. Access URL by Browser
```
curl http://127.0.0.1/test.rb
```
or
```
curl http://127.0.0.1/mruby-test
```

### Manual Build      
* mruby/mruby build

        cd mod_mruby
        git submodule init
        git submodule update
        cd mruby
        rake
        cd ..

* configure

        ./configure

* using mruby/mruby

        make
        make install

## Example
* Selecting vhost area like mod_vhost_alias(hook on translatename)
    ```ruby
    r = Apache::Request.new()
    s = Apache::Server.new()

    r.filename = s.document_root + "/" + r.hostname + "/" + r.uri

    Apache::return(Apache::OK)
    ```
* Proxy balancer like mod_proxy_balancer(hook on translatename)
    ```ruby
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

## Functions
* [Functions Page](https://github.com/matsumoto-r/mod_mruby/wiki/Functions)

## Data Structure
* [Data Structure Page](https://github.com/matsumoto-r/mod_mruby/wiki/Data-Structure)

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php

