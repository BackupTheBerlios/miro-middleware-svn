## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_qt.m4
##
##############################################################################

AC_DEFUN(AC_SEARCH_QT,
[
	ac_with_Qt=no
	ac_Qt_include=$QTDIR/include
	ac_Qt_libdir=$QTDIR/lib
	ac_Qt_bindir=$QTDIR/bin
	ac_Qt_lib=qt

	AC_ARG_WITH(Qt,
		AC_HELP_STRING([--with-Qt=DIR], [root directory of Qt installation]),
		ac_with_Qt=$withval
		if test "x${ac_with_Qt}" != xyes && test "x${ac_with_Qt}" != xno; then
			ac_Qt_include="$withval/include"
			ac_Qt_libdir="$withval/lib"
			ac_Qt_bindir="$withval/bin"
		fi
	)

	AC_ARG_WITH([Qt-include-dir],
		AC_HELP_STRING([--with-Qt-include-dir=DIR], [Qt header files are in DIR]),
		ac_with_Qt=$withval
		if test "x${ac_with_Qt}" != xyes && test "x${ac_with_Qt}" != xno; then
			ac_Qt_include="$withval"
		fi
	)
	AC_ARG_WITH([Qt-bin-dir],
		AC_HELP_STRING([--with-Qt-bin-dir=DIR], [Qt utilities such as moc and uic are in DIR]),
		ac_with_Qt=$withval
		if test "x${ac_with_Qt}" != xyes && test "x${ac_with_Qt}" != xno; then
			ac_Qt_bindir="$withval"
		fi
	)
	AC_ARG_WITH([Qt-lib-dir],
		AC_HELP_STRING([--with-Qt-lib-dir=DIR], [The Qt library is in DIR]),
		ac_with_Qt=$withval
		if test "x${ac_with_Qt}" != xyes && test "x${ac_with_Qt}" != xno; then
			ac_Qt_libdir="$withval"
		fi
	)
	AC_ARG_WITH([Qt-lib],
		AC_HELP_STRING([--with-Qt-lib=LIB], [Use -lLIB to link with the Qt library]),
		ac_with_Qt=$withval
		if test "x${ac_with_Qt}" != xyes && test "x${ac_with_Qt}" != xno; then
			ac_Qt_lib="$withval"
		fi
	)

	AC_SEARCH_QT_LOCAL($ac_Qt_libdir, $ac_Qt_include, $ac_Qt_bindir, $ac_Qt_lib)
])




# subfunction for easier reuse
AC_DEFUN(AC_SEARCH_QT_LOCAL,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS
	OLD_PATH=$PATH

	if test "$1"; then
		LDFLAGS="$LDFLAGS -L$1 -l$4"
	fi
	if test "$2"; then
		CPPFLAGS="$CPPFLAGS -I$2"
		CFLAGS="$CFLAGS -I$2"
	fi
	if test "$3"; then
		PATH="$3:$PATH"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Qt)
	AC_TRY_LINK([
		#include <qapplication.h>
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
		AC_MSG_ERROR([Qt not (properly) installed. Get a (free) recent version from: http://www.trolltech.com. Please check your installation! For more details about this problem, look at the end of config.log.])
	else
		if test "$1"; then
			AC_SUBST(QT_LDFLAGS, "-L$1")
		fi
		if test "$2"; then
			AC_SUBST(QT_CPPFLAGS, "-I$2")
			AC_SUBST(QT_CFLAGS, "-I$2")
		fi
		if test "$4"; then
			AC_SUBST(QT_LIBS, "-l$4")
		fi
	fi

# moc test
	# AC_CHECK_PROG(ac_qtmocdir, moc, yes)
	AC_PATH_PROG(ac_qtmocdir, moc, no)
	if test "x$ac_qtmocdir" != xno; then
		AC_SUBST(QT_MOC, "$ac_qtmocdir")
	else
		AC_MSG_ERROR([Qt not (properly) installed. Get a (free) recent version from: http://www.trolltech.com. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi

# uic test
	# AC_CHECK_PROG(ac_qtuicdir, uic, yes)
	AC_PATH_PROG(ac_qtuicdir, uic, no)
	if test "x$ac_qtuicdir" != xno; then
		AC_SUBST(QT_UIC, "$ac_qtuicdir")
	else
		AC_MSG_ERROR([Qt not (properly) installed. Get a (free) recent version from: http://www.trolltech.com. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi

	PATH=$OLD_PATH
])
