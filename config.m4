dnl $Id$
dnl config.m4 for extension geoip

dnl Contributed by Jonathan Whiteman of cyberflowsolutions.com

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(geoip, for geoip support,
dnl Make sure that the comment is aligned:
[  --with-geoip             Include GeoIP support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(geoip, whether to enable geoip support,
dnl Make sure that the comment is aligned:
dnl [  --enable-geoip           Enable geoip support])

if test "$PHP_GEOIP" != "no"; then
  dnl Write more examples of tests here...

  AC_MSG_CHECKING([check the PHP mininum version for chardet (>=4.3.0 or <7.0.0)])
  if test -z $phpincludedir ; then
    phpincludedir=$prefix"/include/php"
  fi

  PHP_CHKVER=`
    awk '/^#define PHP_VERSION_ID/ { print $NF; }' $phpincludedir/main/php_version.h 2> /dev/null
  `
  PHP_CURVER=`
    awk '/^#define PHP_VERSION /  { print gensub(/\"/, "", "g", $NF); }' \
      $phpincludedir/main/php_version.h 2> /dev/null
  `
  AC_MSG_RESULT([$PHP_CURVER])
  if test -z $PHP_CHKVER ; then
    IFS="." read MAJORL MINORL PATCHL <<< "$PHP_CURVER"
    printf -v PHP_CHKVER "%d%02d%02d" "$MAJORL" "$MINORL" "$PATCHL"
  fi

  if test -z $PHP_CHKVER || test $PHP_CHKVER -lt 40300 ; then
    AC_MSG_ERROR([The krisp extension is unsupported PHP $PHP_CURVER. Use PHP 4.3.0 or after!])
  fi
  if test -z $PHP_CHKVER || test $PHP_CHKVER -ge 70000 ; then
    AC_MSG_ERROR([The krisp extension is unsupported PHP $PHP_CURVER. Use PHP 5.6 or before!])
  fi

  # --with-geoip -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/GeoIP.h"  # you most likely want to change this
  if test -r $PHP_GEOIP/$SEARCH_FOR; then # path given as parameter
    GEOIP_DIR=$PHP_GEOIP
  else # search default path list
    AC_MSG_CHECKING([for geoip files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        GEOIP_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$GEOIP_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the geoip distribution])
  fi

  # --with-geoip -> add include path
  PHP_ADD_INCLUDE($GEOIP_DIR/include)

  # --with-geoip -> check for lib and symbol presence
  LIBNAME=GeoIP # you may want to change this
  LIBSYMBOL=GeoIP_open # you most likely want to change this

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $GEOIP_DIR/$PHP_LIBDIR, GEOIP_SHARED_LIBADD)
    AC_DEFINE(HAVE_GEOIPLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong geoip lib version or lib not found])
  ],[
    -L$GEOIP_DIR/$PHP_LIBDIR -lm -ldl
  ])

  PHP_SUBST(GEOIP_SHARED_LIBADD)

  extra_src=""

  PHP_NEW_EXTENSION(geoip, php_geoip.c $extra_src, $ext_shared)
fi

