## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_festival.m4
##
##############################################################################

AC_DEFUN(AC_SEARCH_FESTIVAL,
[

	AC_ARG_WITH(Festival,
		AC_HELP_STRING([--with-Festival=DIR], [root directory of Festival installation]),
		ac_with_FESTIVAL=$withval
		if test "x${ac_with_FESTIVAL}" != xyes; then
			FESTIVAL="$withval"
		fi,
		ac_with_FESTIVAL=no
	)

	if test "x${ac_with_FESTIVAL}" != xno ; then
		AC_SEARCH_FESTIVAL_LOCAL($FESTIVAL)
	else
		if test "${FESTIVAL_ROOT}" != "" ; then
			AC_SEARCH_FESTIVAL_LOCAL($FESTIVAL_ROOT)
		else
			AC_SEARCH_FESTIVAL_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN(AC_SEARCH_FESTIVAL_LOCAL,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/src/lib $SPEECHTOOLS_LDFLAGS $SPEECHTOOLS_LIBS -L$1/lib -lFestival"
		CPPFLAGS="$CPPFLAGS $SPEECHTOOLS_CPPFLAGS -I$1/src/include -I/include"
		CFLAGS="$CFLAGS $SPEECHTOOLS_CFLAGS -I$1/src/include -I/include"
	else
		LDFLAGS="$LDFLAGS $SPEECHTOOLS_LDFLAGS $SPEECHTOOLS_LIBS -lFestival"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Festival)
	AC_TRY_LINK([
		#include <festival.h>
	],[
		;
	],[
	success=yes
	],[
	success=no
	])
	AC_MSG_RESULT($success)
	AC_LANG_POP()

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

	if test "x$success" != xyes; then
		AC_MSG_WARN(Festival not (properly) installed. See http://www.cstr.ed.ac.uk/projects/festival.html)
		has_festival=no
	else
		AC_SUBST(FESTIVAL_LIBS, "-lFestival")
		if test "$1" ; then
			AC_SUBST(FESTIVAL_ROOT, "$1")
			AC_SUBST(FESTIVAL_LDFLAGS, "-L$1/src/lib -L$1/lib")
			AC_SUBST(FESTIVAL_CPPFLAGS, "-I$1/src/include -I/include")
			AC_SUBST(FESTIVAL_CFLAGS, "-I$1/src/include -I/include")
		fi
		has_festival=yes
	fi
])
