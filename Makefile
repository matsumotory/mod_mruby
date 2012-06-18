##
##  Makefile -- Build procedure for sample mod_mruby Apache module
##	  MATSUMOTO, Ryosuke
##

# target module source
TARGET=mod_mruby.c ap_mrb_request.c ap_mrb_string.c ap_mrb_utils.c ap_mrb_server.c ap_mrb_init.c

#   the used tools
# use apache 2.2
APXS=/usr/sbin/apxs
APACHECTL=/etc/init.d/httpd

# use apache 2.4
#APXS=/usr/local/apache2.4/bin/apxs
#APACHECTL=/usr/local/apache2.4/bin/apachectl
#DEF=-D__APACHE24__

#   additional user defines, includes and libraries
INC=-I. -I/usr/local/src/mruby/src -I/usr/local/src/mruby/include -I/usr/include/json
LIB=-lm /usr/local/src/mruby/lib/libmruby.a -lm /usr/local/src/mruby/mrblib/mrblib.o -lm /usr/lib/libjson.la
WC=-Wc,-std=c99,-Wall,-Werror-implicit-function-declaration
CFLAGS = $(INC) $(LIB) $(WC)

#   the default target
all: mod_mruby.so

#   compile the DSO file
mod_mruby.so: $(TARGET)
	$(APXS) -c $(DEF) $(CFLAGS) $(TARGET)

#   install the DSO file into the Apache installation
#   and activate it in the Apache configuration
install: all
	$(APXS) -i -a -n 'mruby' .libs/mod_mruby.so

#   cleanup
clean:
	-rm -rf .libs *.o *.so *.lo *.la *.slo *.loT

#   reload the module by installing and restarting Apache
reload: install restart

#   the general Apache start/restart/stop procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

