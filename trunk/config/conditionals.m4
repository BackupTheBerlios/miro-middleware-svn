## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/conditionals.m4
##
##############################################################################

## platforms

AC_DEFUN(AC_DETERMINE_PLATFORMS,
[
AC_ARG_ENABLE(
	B21,
	AC_HELP_STRING([--enable-B21], [Turn on B21 support (default off)]),
	ac_request_b21=$enableval,
	ac_request_b21=no)

AC_ARG_ENABLE(
	Pioneer1,
	AC_HELP_STRING([--enable-Pioneer1], [Turn on Pioneer1 support (default on)]),
	ac_request_pioneer1=$enableval,
	ac_request_pioneer1=yes)

AC_ARG_ENABLE(
	Sparrow99,
	AC_HELP_STRING([--enable-Sparrow99], [Turn on Sparrow99 support (default on)]),
	ac_request_sparrow99=$enableval,
	ac_request_sparrow99=yes)

AC_ARG_ENABLE(
	Deprecated,
	AC_HELP_STRING([--enable-Deprecated], [Turn on support for deprecated interfaces]),
	ac_request_deprecated=$enableval,
	ac_request_deprecated=no)
])

## video devices

AC_DEFUN(AC_DETERMINE_VIDEODEVICES,
[
AC_ARG_ENABLE(
	BTTV,
	AC_HELP_STRING([--enable-BTTV], [Turn on BTTV support (default on)]),
	ac_request_bttv=$enableval,
	ac_request_bttv=yes)

AC_ARG_ENABLE(
	Meteor,
	AC_HELP_STRING([--enable-Meteor], [Turn on Meteor support (default off)]),
	ac_request_meteor=$enableval,
	ac_request_meteor=no)

AC_ARG_ENABLE(
	IEEE1394,
	AC_HELP_STRING([--enable-IEEE1394], [Turn on IEEE1394 support (default on)]),
	ac_request_ieee1394=$enableval, 
	ac_request_ieee1394=yes)
])


## additional stuff (e.g. documentation)

AC_DEFUN(AC_DETERMINE_ADDITIONAL,
[
AC_ARG_ENABLE(
	Documentation,
	AC_HELP_STRING([--enable-Documentation], [Build the documentation (default off)]),
	ac_request_docu=$enableval,
	ac_request_docu=no)

AC_ARG_ENABLE(
	Speech,
	AC_HELP_STRING([--enable-Speech], [Turn on Speech support (default off)]),
	ac_request_speech=$enableval,
	ac_request_speech=no)
])


AC_DEFUN(AC_DETERMINE_GCC_VERSION,
[
if test "x$GCC" = "xyes"; then
	dnl cut out the version number (additional (blank) lines are eliminated)
	dnl ac_gcc_version=`for i in \`$CC --version\`; do expr $i : '\([[0-9]]\+\(\.[[0-9]]\+\)\+\)'; done | grep -v ^$`
	ac_gcc_version=`$CC -dumpversion`
	ac_gcc_major=`echo $ac_gcc_version | cut -d . -f 1 -`
	ac_gcc_minor=`echo $ac_gcc_version | cut -d . -f 2 -`
else
	ac_gcc_version=
fi

AH_TEMPLATE([GCC_VERSION], [the used gcc version.])
AH_TEMPLATE([GCC_MAJOR_VERSION], [the major number of the used gcc version.])
AH_TEMPLATE([GCC_MINOR_VERSION], [the minor number of the used gcc version.])

if test "x$ac_gcc_version" != "x"; then
	AC_DEFINE_UNQUOTED(GCC_VERSION, $ac_gcc_version)
	AC_DEFINE_UNQUOTED(GCC_MAJOR_VERSION, $ac_gcc_major)
	AC_DEFINE_UNQUOTED(GCC_MINOR_VERSION, $ac_gcc_minor)
fi
])
