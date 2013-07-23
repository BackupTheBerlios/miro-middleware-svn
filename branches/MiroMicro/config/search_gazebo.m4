## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id: search_gazebo.m4 563 2006-08-03 21:19:02Z gbroten $
##
## Miro/config/search_gazebo.m4
##
##############################################################################

AC_DEFUN([AC_SEARCH_GAZEBO],
[

        AC_ARG_WITH(Gazebo,
                AC_HELP_STRING([--with-Gazebo=DIR], [root directory of Gazebo installation]),
		ac_with_GAZEBO=$withval
		if test "x${ac_with_GAZEBO}" != xyes; then
			GAZEBO="$withval"
		fi,
		ac_with_GAZEBO=no
	)

	ac_has_gazebo=no

	if test "x${ac_with_GAZEBO}" != xno ; then
		AC_SEARCH_GAZEBO_LOCAL($GAZEBO)
	else
		if test "${GAZEBO_ROOT}" != "" ; then
			AC_SEARCH_GAZEBO_LOCAL($GAZEBO_ROOT)
		else
			AC_SEARCH_GAZEBO_LOCAL("/usr/local")
		fi
	fi
#	AC_MSG_RESULT($ac_has_gazebo)
	if test x$ac_has_gazebo == "xno"; then
		AC_MSG_ERROR([GAZEBO not (properly) installed. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi
])




# subfunction for easier reuse
AC_DEFUN([AC_SEARCH_GAZEBO_LOCAL],
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/lib -lgazebo"
		CPPFLAGS="$CPPFLAGS -I$1/include"
		CFLAGS="$CFLAGS -I$1/include"
	else
		LDFLAGS="$LDFLAGS -lgazebo"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for GAZEBO)
	AC_TRY_LINK([
		#include <gazebo/gazebo.h>
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

	GAZEBO_LDFLAGS=""
	GAZEBO_LIBS="-lgazebo"
	GAZEBO_CFLAGS=""

	if test "x$success" != xyes; then
		AC_MSG_WARN([GAZEBO not (properly) installed.])
		ac_has_gazebo=no
	else
		AC_SUBST(GAZEBO_LIBS, "-lgazebo")
		if test "$1" ; then
			AC_SUBST(GAZEBO_ROOT, "$1")
			AC_SUBST(GAZEBO_LDFLAGS, "-L$1/lib")
			AC_SUBST(GAZEBO_CPPFLAGS, "-I$1/include")
			AC_SUBST(GAZEBO_CFLAGS, "-I$1/include")
		fi
		ac_has_gazebo=yes
	fi
])
