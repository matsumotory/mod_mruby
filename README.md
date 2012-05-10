## What's mod_mruby
mod_mruby - to provide an alternative to mod_lua.

Apache modules can be implemeted by mruby scripts on Apache HTTP Server installed mod_mruby.

## How to use
### Compile
    make

### Install
    make install

### Settings
* Add to /usr/local/apache/conf/httpd.conf

        LoadModule mruby_module modules/mod_mruby.so
        # remove comment out if you use .mrb as Web contens
        # AddHandler mruby-script .mrb

    * hook mrb-script on ap_hook_translateName First phase

             mrubyTranslateNameFirst /path/to/file.mrb

    * hook mrb-script on ap_hook_translateName Middle phase

             mrubyTranslateNameMiddle /path/to/file.mrb

    * hook mrb-script on ap_hook_translateName last phase

             mrubyTranslateNamelast /path/to/file.mrb

    * hook mrb-script on ap_hook_handler phase

             mrubyHandler /path/to/file.mrb

* Apache restart

        /usr/local/apache/bin/apachectl restart

### Compile, Install and Apache restart
    make reload

## Example and Test
* Added to /usr/local/apache/conf/httpd.conf

        LoadModule mruby_module modules/mod_mruby.so
        mrubyTranslateNameMiddle /usr/local/apache/htdocs/test.mrb

* created /usr/local/apache/htdocs/test.mrb

        require 'Apache'
        
        r = Apache::Request.new()
        r.content_type = "text/html"
        Apache.rputs("test<br>")
        Apache.rputs("OK status= "+Apache::OK+"<BR>")
        Apache.rputs("DECLINED status= "+Apache::DECLINED+"<BR>")
        Apache.rputs("HTTP_OK status= "+Apache::HTTP_OK+"<BR>")
        Apache.rputs("HTTP_SERVICE_UNAVAILABLE status= "+Apache::HTTP_SERVICE_UNAVAILABLE+"<BR>")
        Apache.rputs("filename= "+r.filename+"<BR>")
        Apache.rputs("uri= "+r.uri+"<BR>")
        r.filename = "/usr/local/apache2.4/htdocs/index.html"
        r.uri = "/ndex.html"
        Apache.rputs("---- request_rec changed ----<br>")
        Apache.rputs("filename= "+r.filename+"<BR>")
        Apache.rputs("uri= "+r.uri+"<BR>")
        Apache.rputs("content_type= "+r.content_type+"<BR>")
        
        Apache.return(Apache::OK)

* access to http://example.com/test.mrb

-------- following readme is old -------
mod_mruby - to provide an alternative to mod_lua

- Compile

make

- Install

make install

- Settings

cat /etc/httpd/conf.d/mruby.so
    LoadModule mruby_module modules/mod_mruby.so
    mrubyHandler /var/www/html/sleep.mrb

/etc/init.d/httpd restart

- Experiment 1
    cat /var/www/html/sleep.mrb
        require 'Apache'
        Apache.sleep(5)

    access to http://example.com/
    sleeping 5 is success.

- Experiment 2
    cat /var/www/html/hello.mrb
        require 'Apache'
        Apache.rputs("<h5>hellow mod_mruby world!</h5>")

    access to http://example.com/
    output "hellow mod_mruby world!" by h5 tag on your browser.

- Experiment 3
    cat /var/www/html/syslogger.mrb
        require 'Apache'
        Apache.syslogger("notice", "error!")

    access to http://example.com/
    output "error!" on /var/log/syslog

- Experiment 4
    cat /var/www/html/errlogger.mrb
        require 'Apache'
        Apache.errlogger(5, "this is error")

    access to http://example.com/
    output "this is error" on /var/log/httpd/error_log

    argv1 is the following priority list.

    #define APLOG_EMERG 0   /* system is unusable */
    #define APLOG_ALERT 1   /* action must be taken immediately */
    #define APLOG_CRIT  2   /* critical conditions */
    #define APLOG_ERR   3   /* error conditions */
    #define APLOG_WARNING   4   /* warning conditions */
    #define APLOG_NOTICE    5   /* normal but significant condition */
    #define APLOG_INFO  6   /* informational */
    #define APLOG_DEBUG 7   /* debug-level messages */
