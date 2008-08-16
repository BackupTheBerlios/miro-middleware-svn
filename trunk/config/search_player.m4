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
	PKG_CHECK_MODULES(PLAYER, playerc++, success=yes, success=no)

	if test "x$success" != xyes; then
		OLD_LDFLAGS=$LDFLAGS
		OLD_CFLAGS=$CFLAGS
		OLD_CPPFLAGS=$CPPFLAGS
		if test "$1" ; then
			LDFLAGS="$LDFLAGS -L$1/client_libs -lplayerc++ -L$1/client_libs -lplayerc"
			CPPFLAGS="$CPPFLAGS -I$1/client_libs/libplayerc -I$1/client_libs/libplayerc++ -I$1/client_libs -I$1/"
			CFLAGS="$CFLAGS -I$1/client_libs/libplayerc -I$1/client_libs/libplayerc++ -I$1/client_libs/ -I$1/"
		else
			LDFLAGS="$LDFLAGS -lplayerc++ -lplayerc"
		fi



		AC_LANG_PUSH(C++)
		AC_MSG_CHECKING(for Player)
		AC_TRY_LINK([
			#include <player/playerc++.h>
		],[
			;
		],[
		success=yes
		],[
		success=yes
		])
		AC_MSG_RESULT($success)
		AC_LANG_POP()

		LDFLAGS=$OLD_LDFLAGS
		CPPFLAGS=$OLD_CPPFLAGS
		CFLAGS=$OLD_CFLAGS

		PLAYER_LDFLAGS="-L$1/lib"
		PLAYER_LIBS="-lplayerc++ -lboost_signals -lboost_thread -lplayerc -lm -lplayerxdr -lplayererror"
		PLAYER_CFLAGS="-I$1/client_libs/libplayerc -I$1/client_libs/libplayerc++ -I$1/client_libs -I$1/"
	fi

	if test "x$success" != xyes; then
		AC_MSG_WARN([Player not (properly) installed. See http://playerstage.sourceforge.net])
		ac_has_player=no
	else
		PLAYER_CPPFLAGS=$PLAYER_CFLAGS
		AC_SUBST(PLAYER_LIBS)
		AC_SUBST(PLAYER_LDFLAGS)
		AC_SUBST(PLAYER_CFLAGS)
		AC_SUBST(PLAYER_CPPFLAGS)
		ac_has_player=yes
	fi
])
