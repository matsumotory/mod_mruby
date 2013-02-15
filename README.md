# Welcome to the mod_mruby wiki!
## What's mod_mruby
mod_mruby - to provide an alternative to mod_lua.

Apache modules can be implemeted by mruby scripts on Apache HTTP Server 2.2/2.4 installed mod_mruby.

Supported MPM: worker prefork event

Supported OS: Linux FreeBSD Windows and so on.

## How to use
### 1. Download
    git clone git://github.com/matsumoto-r/mod_mruby.git

### 2. Auto Build
    cd mod_mruby
    sh build.sh

### 3. Test Settings
* Add to httpd.conf

        LoadModule mruby_module modules/mod_mruby.so
        AddHandler mruby-script .rb

* test.rb copy   

        cp -p test/test.rb $(APACHE_DOCMENT_ROOT)/.

### 4. Apache Restart
    service httpd restart

### 5. Access URL by Browser
    curl http://127.0.0.1/test.rb

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

