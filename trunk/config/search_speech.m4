## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_speed.m4
##
##############################################################################

AC_DEFUN([AC_SEARCH_SPEECHTOOLS],
[
	AC_ARG_WITH(Speechtools,
		AC_HELP_STRING([--with-Speechtools=DIR], [root directory of Speechtools installation]),
		ac_with_SPEECHTOOLS=$withval
		if test "x${ac_with_SPEECHTOOLS}" != xyes; then
			SPEECHTOOLS="$withval"
		fi,
		ac_with_SPEECHTOOLS=no
	)

	if test "x${ac_with_SPEECHTOOLS}" != xno ; then
		AC_SEARCH_SPEECHTOOLS_LOCAL($SPEECHTOOLS)
	else
		if test "${SPEECH_TOOLS_ROOT}" != "" ; then
			AC_SEARCH_SPEECHTOOLS_LOCAL($SPEECH_TOOLS_ROOT)
		else
			AC_SEARCH_SPEECHTOOLS_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN([AC_SEARCH_SPEECHTOOLS_LOCAL],
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/src/lib -L$1/lib -lestools -leststring -lestbase -ltermcap"
		CPPFLAGS="$CPPFLAGS -I$1/src/include -I$1/include"
		CFLAGS="$CFLAGS -I$1/src/include -I$1/include"
	else
		LDFLAGS="$LDFLAGS -lestools -leststring -lestbase -ltermcap"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Speechtools)
	AC_TRY_LINK([
		#include <EST.h>
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
		AC_MSG_WARN([Speechtools not (properly) installed. See http://www.cstr.ed.ac.uk/projects/speech_tools])
		ac_has_speechtools=no
	else
		AC_SUBST(SPEECHTOOLS_LIBS, "-lestools -leststring -lestbase -ltermcap")
		if test "$1" ; then
			AC_SUBST(SPEECHTOOLS_ROOT, "$1")
			AC_SUBST(SPEECHTOOLS_LDFLAGS, "-L$1/src/lib -L$1/lib")
			AC_SUBST(SPEECHTOOLS_CPPFLAGS, "-I$1/src/include -I$1/include")
			AC_SUBST(SPEECHTOOLS_CFLAGS, "-I$1/src/include -I$1/include")
		fi
		ac_has_speechtools=yes
	fi
])
