## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/search_player.m4
##
##############################################################################

AC_DEFUN([AC_SEARCH_PLAYER],
[

	AC_ARG_WITH(Player,
		AC_HELP_STRING([--with-Player=DIR], [root directory of Player installation]),
		ac_with_PLAYER=$withval
		if test "x${ac_with_PLAYER}" != xyes; then
			PLAYER="$withval"
		fi,
		ac_with_PLAYER=no
	)

	if test "x${ac_with_PLAYER}" != xno ; then
		AC_SEARCH_PLAYER_LOCAL($PLAYER)
	else
		if test "${PLAYER_ROOT}" != "" ; then
			AC_SEARCH_PLAYER_LOCAL($PLAYER_ROOT)
		else
			AC_SEARCH_PLAYER_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN([AC_SEARCH_PLAYER_LOCAL],
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS -L$1/lib -lplayerclient"
		CPPFLAGS="$CPPFLAGS -I$1/include"
		CFLAGS="$CFLAGS -I$1/include"
	else
		LDFLAGS="$LDFLAGS -lplayerclient"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Player)
	AC_TRY_LINK([
		#include <player.h>
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
		AC_MSG_WARN([Player not (properly) installed. See http://playerstage.sourceforge.net])
		ac_has_player=no
	else
		AC_SUBST(PLAYER_LIBS, "-lplayerclient")
		if test "$1" ; then
			AC_SUBST(PLAYER_ROOT, "$1")
			AC_SUBST(PLAYER_LDFLAGS, "-L$1/lib")
			AC_SUBST(PLAYER_CPPFLAGS, "-I$1/include")
			AC_SUBST(PLAYER_CFLAGS, "-I$1/include")
		fi
		ac_has_player=yes
	fi
])
