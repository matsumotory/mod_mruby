##
##  Makefile -- Build procedure for sample mod_mruby Apache module
##	  MATSUMOTO, Ryosuke
##

# target module source
TARGET=mod_mruby.c ap_mrb_*.c

#   the used tools for Apache 2.2
APXS=/usr/sbin/apxs
APACHECTL=/etc/init.d/httpd

#   the used tools and define for Apache 2.4
#$APXS=/usr/local/apache243/bin/apxs
#APACHECTL=/usr/local/apache243/bin/apachectl
#DEF=-D__APACHE24__

#   additional user defines, includes and libraries
INC=-I. -I./vendors/src -I./vendors/include
LIB=-lm ./vendors/lib/libmruby.a ./vendors/mrblib/mrblib.o
WC=-Wc,-std=c99,-Wall,-Werror-implicit-function-declaration
CFLAGS = $(INC) $(LIB) $(WC)

#   the default target
all: libmruby.a mod_mruby.so

#   build for iij extended lib
extend: libmruby-ex.a mod_mruby.so

#   compile the DSO file
mod_mruby.so: $(TARGET)
	$(APXS) -c $(DEF) $(CFLAGS) $(TARGET)

#   install the DSO file into the Apache installation
#   and activate it in the Apache configuration
install:
	$(APXS) -i -a -n 'mruby' .libs/mod_mruby.so

#   cleanup
clean:
	-rm -rf .libs *.o *.so *.lo *.la *.slo *.loT tmp vendors

#   reload the module by installing and restarting Apache
reload: install restart

#   the general Apache start/restart/stop procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

# libmruby.a
tmp/mruby:
	mkdir -p tmp vendors
	cd tmp; git clone git://github.com/mruby/mruby.git

libmruby.a: tmp/mruby
	cd tmp/mruby && make CFLAGS="-O3 -fPIC"
	cp -r tmp/mruby/include vendors/
	cp -r tmp/mruby/lib vendors/
	cp -r tmp/mruby/src vendors/
	cp -r tmp/mruby/bin vendors/
	cp -r tmp/mruby/mrblib vendors/

# libmruby.a (+iij extended lib)
tmp/mruby-ex:
	mkdir -p tmp vendors
	cd tmp; git clone git://github.com/iij/mruby.git

libmruby-ex.a: tmp/mruby-ex
	cd tmp/mruby && make CFLAGS="-O3 -fPIC"
	cp -r tmp/mruby/include vendors/
	cp -r tmp/mruby/lib vendors/
	cp -r tmp/mruby/src vendors/
	cp -r tmp/mruby/bin vendors/
	cp -r tmp/mruby/mrblib vendors/
