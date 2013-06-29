#!/bin/sh

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
mv build_config.rb{,.orig}
cp ../build_config.rb .
#rake CFLAGS="-g3 -fPIC"
rake
echo "mruby building ... Done"
cd ..
echo "mod_mruby building ..."
#./configure --with-apxs=/usr/local/apache/bin/apxs --with-apachectl=/usr/local/apache/bin/apachectl
./configure
make
sudo make install
echo "mod_mruby building ... Done"
echo "build.sh ... successful"

#make restart
