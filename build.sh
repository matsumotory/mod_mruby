#!/bin/sh

set -e

cd mruby
if [ -d "./build" ]; then
    rake clean
fi
rake CFLAGS="-fPIC"
cd ..
./configure
make
make install
make restart
