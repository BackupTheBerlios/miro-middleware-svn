## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_ace.m4
##
##############################################################################

AC_DEFUN([AC_SEARCH_ACE],
[
	AC_ARG_WITH(ACE,
		AC_HELP_STRING([--with-ACE=DIR], [root directory of ACE installation]),
		ac_with_ACE=$withval
		if test "x${ac_with_ACE}" != xyes; then
			ACE="$withval"
		fi,
		ac_with_ACE=no
	)

	if test "x${ac_with_ACE}" != xno ; then
		AC_SEARCH_ACE_LOCAL($ACE)
	else
		if test "${ACE_ROOT}" != "" ; then
			AC_SEARCH_ACE_LOCAL($ACE_ROOT)
		else
			AC_SEARCH_ACE_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN([AC_SEARCH_ACE_LOCAL],
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/ace -lACE"
		CPPFLAGS="$CPPFLAGS -I$1 -D_GNU_SOURCE"
		CFLAGS="$CFLAGS -I$1"
	else
                CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
		LDFLAGS="$LDFLAGS -lACE"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for ACE)
	AC_TRY_LINK([
		#include <ace/OS.h>
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
		AC_MSG_ERROR([ACE not (properly) installed. Source tarball and CVS at: http://www.cs.wustl.edu/~schmidt/ACE.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	else
		AC_SUBST(ACE_LIBS, "-lACE")
		if test "$1" ; then
			AC_SUBST(ACE_ROOT, "$1")
			AC_SUBST(ACE_LDFLAGS, "-L$1/ace")
			AC_SUBST(ACE_CPPFLAGS, "-I$1 -D_GNU_SOURCE")
			AC_SUBST(ACE_CFLAGS, "-I$1")
		fi
	fi
])
