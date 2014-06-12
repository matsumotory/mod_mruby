FROM stackbrew/ubuntu:14.04
#
MAINTAINER matsumotory
#
RUN apt-get -y update
RUN apt-get -y install sudo openssh-server
RUN apt-get -y install git
RUN apt-get -y install apache2 apache2-dev apache2-utils
RUN apt-get -y install rake
RUN apt-get -y install ruby2.0 ruby2.0-dev
RUN apt-get -y install bison
RUN apt-get -y install libcurl4-openssl-dev
RUN apt-get -y install libhiredis-dev
RUN apt-get -y install libmarkdown2-dev
RUN apt-get -y install libcap-dev
RUN apt-get -y install libcgroup-dev
#
RUN cd /usr/local/src/ && git clone git://github.com/matsumoto-r/mod_mruby.git
RUN cd /usr/local/src/mod_mruby && sh test.sh
