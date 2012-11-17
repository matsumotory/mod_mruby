#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_CONFIG_SRCDIR([ap_mrb_init.h])
#AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_STAT
AC_CHECK_FUNCS([memset putenv strtol])

# Checks for apache tools.
AC_PATH_PROG(APXS_PATH, apxs, no,
    /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin:/usr/local/apache2/bin)
if test "$APXS_PATH" = no; then
    AC_PATH_PROG(APXS2_PATH, apxs2, no,
        /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin:/usr/local/apache2/bin)
    if test "$APXS2_PATH" = no; then
        AC_MSG_ERROR([apxs2 not found.])
    else
        APXS_PATH=$APXS2_PATH
    fi
fi

AC_PATH_PROG(APACHECTL_PATH, apachectl, no,
    /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin:/usr/local/apache2/bin)
if test "$APACHECTL_PATH" = no; then
    AC_PATH_PROG(APACHE2CTL_PATH, apache2ctl, no,
        /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin:/usr/local/apache2/bin)
    if test "$APACHE2CTL_PATH" = no; then
        AC_MSG_ERROR([apache2ctl not found.])
    else
        APACHECTL_PATH=$APACHE2CTL_PATH
    fi
fi

APACHE_VERSION=`$APACHECTL_PATH -v | awk "/Server version/ {print $ 3}"`
NEED_DEFINE_VERSITON="Apache/2.4.*"

case $APACHE_VERSION in
    $NEED_DEFINE_VERSITON) 
        VERSION_DEF="-D__APACHE24__"
        AC_SUBST(VERSION_DEF)
        echo "checking for apache version... $APACHE_VERSION set VERSION_DEF=$VERSION_DEF"
    ;;
    *)
        echo "checking for apache version... $APACHE_VERSION"
    ;;
esac

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
