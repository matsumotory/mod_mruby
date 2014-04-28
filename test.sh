#!/bin/sh

# ENV example
#
#   APXS_PATH_ENV='--with-apxs=/usr/local/apache/bin/apxs' APACHECTL_PATH_ENV='--with-apachectl=/usr/local/apache/bin/apachectl' sh  test.sh
#

set -e

APXS_PATH=''
if [ $APXS_PATH_ENV ]; then
    APXS_PATH=$APXS_PATH_ENV
fi

APACHECTL_PATH=''
if [ $APACHECTL_PATH_ENV ]; then
    APACHECTL_PATH=$APACHECTL_PATH_ENV
fi

echo "apxs="$APXS_PATH "apachectl="$APACHECTL_PATH

if [ ! -d "./mruby/src" ]; then
    echo "mruby Downloading ..."
    git submodule init
    git submodule update
    echo "mruby Downloading ... Done"
fi

cd mruby
if [ -d "./build" ]; then
    echo "mruby Cleaning ..."
    ./minirake clean
    echo "mruby Cleaning ... Done"
fi

echo "mruby building ..."
mv build_config.rb build_config.rb.orig
cp ../build_config.rb .
BUILD_TYPE='debug' ./minirake
echo "mruby building ... Done"
cd ..

echo "mod_mruby building ..."
./configure $APXS_PATH $APACHECTL_PATH
make
echo "mod_mruby building ... Done"

echo "mod_mruby testing ..."
sudo TEST_USER=$USER make test
echo "mod_mruby testing ... Done"

echo "test.sh ... successful"
