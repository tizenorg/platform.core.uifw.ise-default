# Process this file with autoconf to produce a configure script.
AC_INIT(ise-default, 0.3.002, dalton.lee@samsung.com)
AC_CONFIG_SRCDIR([src/ise.cpp])

ELM_VERSION=0.0.1

AC_SUBST(ELM_VERSION)

# libtool versioning for ISF ise-default

# increment if the interface has additions, changes, removals.
ISF_DEFAULT_CURRENT=0

# increment any time the source changes; set to 0 if you increment CURRENT
ISF_DEFAULT_REVISION=0

# increment if any interfaces have been added; set to 0
# if any interfaces have been removed. removal has 
# precedence over adding, so set to 0 if both happened.
ISF_DEFAULT_AGE=0

AC_SUBST(ISF_DEFAULT_CURRENT)
AC_SUBST(ISF_DEFAULT_REVISION)
AC_SUBST(ISF_DEFAULT_AGE)

GETTEXT_PACKAGE=ise-default
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [The gettext domain])

AC_CANONICAL_HOST

# Init automake stuff
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

# Init gettext
ALL_LINGUAS="ko_KR en zh_CN zh_HK zh_TW de_DE nl_NL es_ES pt_PT el_GR it_IT fr_FR tr_TR ja_JP ru_RU"
AM_GNU_GETTEXT

AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_DISABLE_STATIC
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

AC_SUBST(LIBTOOL_DEPS)

AC_PROG_CXX
AC_PROG_CC
AC_LANG(C++)

AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Check elementary
PKG_CHECK_MODULES(ELM, [elementary >= 0.1.0.0])

# Check SLP-utilX library
PKG_CHECK_MODULES(SLP_UTILX, [utilX >= 0.0.0.1])

# Check SLP-util library
PKG_CHECK_MODULES(HEYNOTI, [heynoti >= 0.0.0.1])

# Check ecore_imf
PKG_CHECK_MODULES(IMF, [ecore-imf >= 0.0.0.1])

# Check ecore-x
PKG_CHECK_MODULES(ECOREX, [ecore-x >= 0.0.0.1])

# Check glib library
PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 0.0.0.1])

# Check vconf library
PKG_CHECK_MODULES(VCONF, [vconf >= 0.0.0.1])

# Check sensor library
PKG_CHECK_MODULES(SENSOR, [sensor >= 0])

# Check isf library
PKG_CHECK_MODULES(ISF, [isf >= 0])

# Check X11 library
PKG_CHECK_MODULES(X11, [x11 >= 0])

# Checks for header files.
AC_HEADER_STDC

# libtool option to control which symbols are exported
# right now, symbols starting with _ are not exported
LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

# Check if we have gthread
#PKG_CHECK_MODULES(GTHREAD2,[gthread-2.0 >= 2.0.0],
#			[HAS_GTHREAD2=yes],
#			[HAS_GTHREAD2=no])

case $host_cpu in
  *arm* ) TARGET=ARM;;
      * ) TARGET=X86;;
esac
AM_CONDITIONAL(ARM, test x$TARGET = xARM)

# Extra args.
AC_ARG_ENABLE(debug,
	      [  --enable-debug            Turn on debugging],
       	      enable_debug=yes,
	      enable_debug=no)

if test "$enable_debug" = "yes"; then
  AC_DEFINE(ENABLE_DEBUG,1,[Define this to enable the debug facility])
  CFLAGS="$CFLAGS -g"
  CXXFLAGS="$CXXFLAGS -g"
fi

ISF_ICONDIR=`$PKG_CONFIG --variable=icondir scim`
ISF_MODULEDIR=`$PKG_CONFIG --variable=moduledir scim`
ISF_DATADIR=`$PKG_CONFIG --variable=scimdatadir scim`
ISF_LIBEXECDIR=`$PKG_CONFIG --variable=moduledir scim`
ISF_INCLUDEDIR=`$PKG_CONFIG --variable=includedir scim`
ISF_LIBDIR=`$PKG_CONFIG --variable=libdir scim --libs elementary`
FACTORYFS_DIR="${FACTORYFS}"
PREFIX_DIR="${PREFIX}"
ISE_NAME="ise-default"

if test "x$ISF_ICONDIR" = "x"; then
  ISF_ICONDIR=${datadir}/scim/icons
fi

if test "x$ISF_MODULEDIR" = "x"; then
  ISF_MODULEDIR=${libdir}/scim-1.0
fi

if test "x$ISF_DATADIR" = "x"; then
  ISF_DATADIR=${datadir}/scim
fi

if test "x$ISF_LIBEXECDIR" = "x"; then
  ISF_LIBEXECDIR=${libdir}/scim-1.0
fi

if test "x$ISF_INCLUDEDIR" = "x"; then
  ISF_INCLUDEDIR=${includedir}/scim-1.0
else
  ISF_INCLUDEDIR=${ISF_INCLUDEDIR}/scim-1.0
fi

if test "x$ISF_LIBDIR" = "x"; then
  ISF_LIBDIR=${libdir}
fi

AC_SUBST(ISF_ICONDIR)
AC_SUBST(ISF_MODULEDIR)
AC_SUBST(ISF_DATADIR)
AC_SUBST(ISF_LIBEXECDIR)
AC_SUBST(ISF_INCLUDEDIR)
AC_SUBST(ISF_LIBDIR)
AC_SUBST(FACTORYFS_DIR)
AC_SUBST(PREFIX_DIR)
AC_SUBST(ISE_NAME)

ISF_DEFAULT_DATADIR=$ISF_DATADIR/ise-default
ISF_DEFAULT_USER_DATADIR=/.scim/ise-default

AC_SUBST(ISF_DEFAULT_DATADIR)
AC_SUBST(ISF_DEFAULT_USER_DATADIR)

# Extra arg for autoscroll
#AC_ARG_ENABLE(autoscroll-check, [ --enable-autoscroll-check        Turn on autoscroll check], autoscroll_check=yes, autoscroll_check=no)
autoscroll_check="yes"
if test "x${autoscroll_check}" = "xyes"; then
	AC_DEFINE(HAVE_CONFORMANT_AUTOSCROLL,1,[Define this to enable autoscroll checking])
	echo "AUTOSCROLL is enabled!"
else
	echo "AUTOSCROLL is disabled!"
fi
AC_CONFIG_FILES([Makefile po/Makefile.in m4/Makefile 
		 data/Makefile
		 data/pixmaps/Makefile
		 src/Makefile])
#		 ise-default.spec])
AC_OUTPUT
