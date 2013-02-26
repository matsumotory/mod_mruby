# Apache HTTP Server Status Realtime Moniter by mod_mruby
Implemented in ApacheCon NA 2013 Hackathon
## Author
- MATSUMOTO Ryosuke
- TSUZAKI Yoshiharu
- OKAMOTO Hiroki

## Install
### Install mod_mruby

see [mod_mruby](https://github.com/matsumoto-r/mod_mruby).

### httpd.conf
    LoadModule mruby_module modules/mod_mruby.so
    mrubyTranslateNameFirst "/tmp/api_request.rb"

### file setteing
    git clone git://github.com/matsumoto-r/mod_mruby_hackathon.git
    cp api/api_request.rb /tmp/.
    touch #{document_root}/dummy

## Start
    cd interface
    node app.js

## Access
http://127.0.0.1:3000/

## Sample
![sample_display](https://github.com/matsumoto-r/mod_mruby_hackathon/blob/master/interface/views/sample_image.jpeg)

## License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php

