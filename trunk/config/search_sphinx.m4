## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_sphinx.m4
##
##############################################################################

AC_DEFUN(AC_SEARCH_SPHINX,
[
	AC_ARG_WITH(Sphinx,
		AC_HELP_STRING([--with-Sphinx=DIR], [root directory of Sphinx installation]),
		ac_with_SPHINX=$withval
		if test "x${ac_with_SPHINX}" != xyes; then
			SPHINX="$withval"
		fi,
		ac_with_SPHINX=no
	)

	if test "x${ac_with_SPHINX}" != xno ; then
		AC_SEARCH_SPHINX_LOCAL($SPHINX)
	else
		if test "${SPHINX_ROOT}" != "" ; then
			AC_SEARCH_SPHINX_LOCAL($SPHINX_ROOT)
		else
			AC_SEARCH_SPHINX_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN(AC_SEARCH_SPHINX_LOCAL,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/src/lib -L$1/lib -lsphinx2 -lsphinx2ad -lsphinx2fe"
		CPPFLAGS="$CPPFLAGS -I$1/src/include -I$1/include"
		CFLAGS="$CFLAGS -I$1/src/include -I$1/include"
	else
		LDFLAGS="$LDFLAGS -lsphinx2 -lsphinx2ad -lsphinx2fe"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Sphinx)
	AC_TRY_LINK([
		#include <sphinx2/s2types.h>
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
		AC_MSG_WARN(Sphinx not (properly) installed. See http://www.speech.cs.cmu.edu/sphinx/)
		ac_has_sphinx=no
	else
		AC_SUBST(SPHINX_LIBS, "-lsphinx2 -lsphinx2ad -lsphinx2fe")
		if test "$1" ; then
			AC_SUBST(SPHINX_ROOT, "$1")
			AC_SUBST(SPHINX_LDFLAGS, "-L$1/src/lib -L$1/lib")
			AC_SUBST(SPHINX_CPPFLAGS, "-I$1/src/include -I$1/include")
			AC_SUBST(SPHINX_CFLAGS, "-I$1/src/include -I$1/include")
		fi
		ac_has_sphinx=yes
	fi
])
