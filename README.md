# Apache HTTP Server Status Moniter by mod_mruby
Implemtend in ApacheCon NA 2013 Hackathon
- Author
-- MATSUMOTO Ryosuke
-- TSUZAKI Yoshiharu
-- OKAMOTO Hiroki

## Install
- Install mod_mruby

see [mod_mruby](https://github.com/matsumoto-r/mod_mruby).

- httpd.conf

    LoadModule mruby_module modules/mod_mruby.so
    mrubyTranslateNameFirst "/tmp/api_request.rb"

- file setteing
    cp api_request.rb /tmp/.
    touch #{document_root}/dummy

## Start

    cd interface
    node app.js

## Access
http://127.0.0.1:3000/
