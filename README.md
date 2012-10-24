# Welcome to the mod_mruby wiki!
## What's mod_mruby
mod_mruby - to provide an alternative to mod_lua.

Apache modules can be implemeted by mruby scripts on Apache HTTP Server 2.2/2.4 installed mod_mruby.

## How to use
### 1. Download
    git clone git://github.com/matsumoto-r/mod_mruby.git

### 2. Compile
* using mruby/murby
    make


* using iij/mruby
    make extend


### 3. Install
    make install

### 4. Settings
* Add to /usr/local/apache/conf/httpd.conf

        LoadModule mruby_module modules/mod_mruby.so
        # remove comment out if you use .mrb as Web contens
        # AddHandler mruby-script .mrb

    * hook mrb-script on ap_hook_translateName Middle phase

             mrubyTranslateNameMiddle /path/to/file.mrb

### 5. Apache Restart
    /etc/init.d/httpd restart

### 6. Install Test
* [Install Test Page](https://github.com/matsumoto-r/mod_mruby/wiki/Install-Test-Page)

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
