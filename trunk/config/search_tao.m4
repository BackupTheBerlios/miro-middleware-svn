## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_tao.m4
##
##############################################################################

AC_DEFUN([AC_SEARCH_TAO],
[
	AC_ARG_WITH(TAO,
		AC_HELP_STRING([--with-TAO=DIR], [root directory of TAO installation]),
		ac_with_TAO=$withval
		if test "x${ac_with_TAO}" != xyes; then
			TAO="$withval"
		fi,
		ac_with_TAO=no
	)

	if test "x${ac_with_TAO}" != xno ; then
		AC_SEARCH_TAO_LOCAL($TAO)
	else
		if test ${TAO_ROOT} != "" ; then
			AC_SEARCH_TAO_LOCAL($TAO_ROOT)
		else
			AC_SEARCH_TAO_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN([AC_SEARCH_TAO_LOCAL],
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS
	OLD_PATH=$PATH

	if test "$1" ; then
#		LDFLAGS="$LDFLAGS $ACE_LDFLAGS $ACE_LIBS -L$1/tao -L$1/orbsvcs/orbsvcs -lTAO"
		LDFLAGS="$LDFLAGS $ACE_LDFLAGS $ACE_LIBS -lTAO"
		CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -I$1 -I$1/orbsvcs -D_GNU_SOURCE"
		CFLAGS="$CFLAGS $ACE_CFLAGS -I$1 -I$1/orbsvcs"
		PATH="$1/TAO_IDL:$PATH"
	else
		LDFLAGS="$LDFLAGS $ACE_LDFLAGS $ACE_LIBS -lTAO"
		CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -D_GNU_SOURCE"
		CFLAGS="$CFLAGS $ACE_CFLAGS"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for TAO)
	AC_TRY_LINK([
		#include <tao/ORB.h>
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
		AC_MSG_ERROR([TAO not (properly) installed. Source tarball and CVS at: http://www.cs.wustl.edu/~schmidt/ACE.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	else
		AC_SUBST(TAO_LIBS, "-lTAO")
		if test "$1" ; then
			AC_SUBST(TAO_ROOT, "$1")
#			AC_SUBST(TAO_LDFLAGS, "-L$1/tao -L$1/orbsvcs/orbsvcs")
			AC_SUBST(TAO_LDFLAGS, "")
			AC_SUBST(TAO_CPPFLAGS, "-I$1 -I$1/orbsvcs")
			AC_SUBST(TAO_CFLAGS, "-I$1 -I$1/orbsvcs")
		fi
	fi

# tao_idl test
	# AC_CHECK_PROG(ac_taobindir, tao_idl, yes)
	AC_PATH_PROG(ac_taobindir, tao_idl, no)
	if test "x$ac_taobindir" != xno ; then
		AC_SUBST(TAO_IDL, "$ac_taobindir")
	else
		AC_MSG_ERROR([TAO not (properly) installed. Source tarball and CVS at: http://www.cs.wustl.edu/~schmidt/ACE.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi

	PATH=$OLD_PATH
])
