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
			ACE_ROOT="$withval"
		fi,
		ac_with_ACE=no
	)

	ac_search_ace_success=no
	AC_MSG_CHECKING(for ACE)

	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

# let's try some different possible places
# if there is a directory given at the command line or if there is a environment variable
# called TAO_ROOT, use this one
	if test "${ACE_ROOT}" != "" ; then
		LDFLAGS="$LDFLAGS -L$ACE_ROOT/ace -lACE"
		CPPFLAGS="$CPPFLAGS -I$ACE_ROOT -D_GNU_SOURCE"
		CFLAGS="$CFLAGS -I$ACE_ROOT"
		AC_SEARCH_ACE_LOCAL()
		AC_SUBST(ACE_ROOT, "$ACE_ROOT")
		AC_SUBST(ACE_LDFLAGS, "-L$ACE_ROOT/ace")
		AC_SUBST(ACE_CPPFLAGS, "-I$ACE_ROOT -D_GNU_SOURCE")
		AC_SUBST(ACE_CFLAGS, "-I$ACE_ROOT")
		AC_SUBST(ACE_LIBS, "-lACE")
	else
#otherwise try without any additional search directories
		CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
		LDFLAGS="$LDFLAGS -lACE"
		AC_SEARCH_ACE_LOCAL()
		AC_SUBST(ACE_LIBS, "-lACE")
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

# if there is still no ACE found, try all these places but assume, that ACE is actually
# installed (using make install, which is available for never versions)
	if test x$ac_search_ace_success == "xno" ; then
		if test "${ACE_ROOT}" != "" ; then
			LDFLAGS="$LDFLAGS -L$ACE_ROOT/lib -lACE"
			CPPFLAGS="$CPPFLAGS -I$ACE_ROOT/include -D_GNU_SOURCE"
			CFLAGS="$CFLAGS -I$ACE_ROOT/include"
			AC_SEARCH_ACE_LOCAL()
			AC_SUBST(ACE_ROOT, "$ACE_ROOT")
			AC_SUBST(ACE_LDFLAGS, "-L$ACE_ROOT/lib")
			AC_SUBST(ACE_CPPFLAGS, "-I$ACE_ROOT/include -D_GNU_SOURCE")
			AC_SUBST(ACE_CFLAGS, "-I$ACE_ROOT/include")
			AC_SUBST(ACE_LIBS, "-lACE")
		fi
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

# if there is still no ACE found, emit an error message and stop
	AC_MSG_RESULT($ac_search_ace_success)
	if test x$ac_search_ace_success == "xno"; then
		AC_MSG_ERROR([ACE not (properly) installed. Source tarball and CVS at: http://deuce.doc.wustl.edu/Download.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi
])




AC_DEFUN([AC_SEARCH_ACE_LOCAL],
[
	AC_LANG_PUSH(C++)
	AC_TRY_LINK([
		#include <ace/OS.h>
	],[
		;
	],[
	success=yes
	],[
	success=no
	])
	AC_LANG_POP()
	ac_search_ace_success=$success
])
