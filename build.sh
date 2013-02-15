#!/bin/sh

set -e

if [ -d "./mruby/src" ]; then
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
rake CFLAGS="-fPIC"
echo "mruby building ... Done"
cd ..
echo "mod_mruby building ..."
./configure
make
make install
echo "mod_mruby building ... Done"

#make restart
