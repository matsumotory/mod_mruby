#!/bin/sh

set -e

. ./httpd_version

HTTPD_TAR=${HTTPD_VERSION}.tar.gz
APR_TAR=${APR}.tar.gz
APR_UTIL_TAR=${APR_UTIL}.tar.gz
APXS_CHECK_CMD="./build/${HTTPD_VERSION}/apache/bin/apachectl -v"
APXS_PATH_ENV=--with-apxs=./build/${HTTPD_VERSION}/apache/bin/apxs
APACHECTL_PATH_ENV=--with-apachectl=./build/${HTTPD_VERSION}/apache/bin/apachectl

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


echo "${HTTPD_VERSION} building ..."
if [ -d "./build" ]; then
  echo "build directory was found"
else
  mkdir build
fi

cd build/
if [ ! -e ${HTTPD_TAR} ]; then
  wget http://ftp.jaist.ac.jp/pub/apache//httpd/${HTTPD_TAR}
  tar xf ${HTTPD_TAR}
fi

cd ${HTTPD_VERSION}/srclib

if [ ! -e ${APR_TAR} ]; then
  wget http://ftp.jaist.ac.jp/pub/apache//apr/${APR_TAR}
  tar xf ${APR_TAR}
fi

if [ ! -e ${APR_UTIL_TAR} ]; then
  wget http://ftp.jaist.ac.jp/pub/apache//apr/${APR_UTIL_TAR}
  tar xf ${APR_UTIL_TAR}
fi

ln -sf ${APR} apr
ln -sf ${APR_UTIL} apr-util

cd ..
./configure --prefix=`pwd`/apache --with-included-apr ${HTTPD_CONFIG_OPT}
make
make install
cd ../..

echo "mod_mruby testing ..."
echo ${APXS_CHECK_CMD}
${APXS_CHECK_CMD}
./configure $APXS_PATH $APACHECTL_PATH
make
make test
echo "mod_mruby testing ... Done"

echo "test.sh ... successful"
