#!/bin/sh

APXS_PATH='--with-apxs=/usr/local/apache/bin/apxs'
APACHECTL_PATH='--with-apachectl=/usr/local/apache/bin/apachectl'

set -e

if [ ! -d "./mruby/src" ]; then
    echo "mruby Downloading ..."
    git submodule init
    git submodule update
    echo "mruby Downloading ... Done"
fi
cd mruby
if [ -d "./build" ]; then
    echo "mruby Cleaning ..."
    rake clean
    echo "mruby Cleaning ... Done"
fi
echo "mruby building ..."
mv build_config.rb build_config.rb.orig
cp ../build_config.rb .
#rake BUILD_BIT='64'
#rake BUILD_TYPE='debug'
#rake
BUILD_TYPE='debug' ./minirake 
echo "mruby building ... Done"
cd ..
echo "mod_mruby building ..."
./configure $APXS_PATH $APACHECTL_PATH
make
echo "mod_mruby building ... Done"
echo "build.sh ... successful"

#sudo make install
#make restart
