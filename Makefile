##
##  Makefile -- Build procedure for sample mod_mruby Apache module
##      MATSUMOTO, Ryosuke
##

# target module source
TARGET=mod_mruby.c

#   the used tools
APXS=/usr/sbin/apxs
APACHECTL=/etc/ini.d/httpd

#   additional user defines, includes and libraries
#DEF=-D__DEBUG__
INC=-I/usr/local/src/mruby/src -I/usr/local/src/mruby/include
LIB=-lm /usr/local/src/mruby/lib/ritevm.a

#   the default target
all: mod_mruby.so

#   compile the DSO file
mod_mruby.so: $(TARGET)
	$(APXS) -c $(DEF) $(INC) $(LIB) $(TARGET)

#   install the DSO file into the Apache installation
#   and activate it in the Apache configuration
install: all
	$(APXS) -i -a -n 'mruby' .libs/mod_mruby.so

#   cleanup
clean:
	-rm -rf .libs mod_mruby.o mod_mruby.so mod_mruby.lo mod_mruby.la mod_mruby.slo mod_mruby.loT

#   reload the module by installing and restarting Apache
reload: install restart

#   the general Apache start/restart/stop procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

