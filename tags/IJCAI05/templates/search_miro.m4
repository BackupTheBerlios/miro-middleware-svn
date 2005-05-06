AC_DEFUN(AC_SEARCH_MIRO,
[
	AC_ARG_WITH(Miro,
		AC_HELP_STRING([--with-Miro=DIR], [root directory of Miro installation]),
		ac_with_MIRO=$withval
		if test "x${ac_with_MIRO}" != xyes; then
			MIRO="$withval"
		fi,
		ac_with_MIRO=no
	)

	if test "x${ac_with_MIRO}" != xno ; then
		AC_SEARCH_MIRO_LOCAL($MIRO)
	else
		if test "${MIRO_ROOT}" != "" ; then
			AC_SEARCH_MIRO_LOCAL($MIRO_ROOT)
		else
			AC_SEARCH_MIRO_LOCAL()
		fi
	fi
])




# subfunction for easier reuse
AC_DEFUN(AC_SEARCH_MIRO_LOCAL,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	if test "$1" ; then
		LDFLAGS="$LDFLAGS $ACE_LDFLAGS -lACE -lTAO_CosNotification -lTAO_CosNaming -lTAO -lTAO_Strategies -L$1/lib -lmiro -lmiroIDL -lmiroSvc"
		CPPFLAGS="$CPPFLAGS -I$1 -I$1/src -I$1/include"
		CFLAGS="$CFLAGS -I$1 -I$1/src -I$1/include"
	else
		LDFLAGS="$LDFLAGS $ACE_LDFLAGS -lACE -lTAO_CosNotification -lTAO_CosNaming -lTAO -lTAO_Strategies -lmiro -lmiroIDL -lmiroSvc"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Miro)
	AC_TRY_LINK([
		#include <miro/Angle.h>
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
		AC_MSG_ERROR([Miro not (properly) installed. Source tarball and CVS at: http://smart.informatik.uni-ulm.de/MIRO/. Please check your installation! For more details about this problem, look at the end of config.log.])
	else
		AC_SUBST(MIRO_LIBS, "-lmiro")
		if test "$1" ; then
			AC_SUBST(MIRO_ROOT, "$1")
			AC_SUBST(MIRO_LDFLAGS, "-L$1/lib")
			AC_SUBST(MIRO_CPPFLAGS, "-I$1 -I$1/src -I$1/include")
			AC_SUBST(MIRO_CFLAGS, "-I$1 -I$1/src -I$1/include")
		fi
	fi

# MakeParams test

	AC_MSG_CHECKING(for MakeParams)
	# Save the "AC_MSG_RESULT file descriptor" to FD 8.
	exec 8>&AC_FD_MSG
	# Temporarily turn off AC_MSG_RESULT so that the user gets pretty
	# messages.
	exec AC_FD_MSG>/dev/null

	OLD_PATH=$PATH
	AC_CHECK_PROG(ac_mirobindir, MakeParams, yes)
	if test "x$ac_mirobindir" = xyes; then
		AC_SUBST(MIRO_MAKEPARAMS, "MakeParams")
	else
		PATH="$MIRO_ROOT/bin"
		AC_CHECK_PROG(ac_mirobindir, MakeParams, yes)
		if test "x$ac_mirobindir" = xyes; then
			AC_SUBST(MIRO_MAKEPARAMS, "$MIRO_ROOT/bin/MakeParams")
		else
			PATH="$MIRO_ROOT/src/makeParams"
			AC_CHECK_PROG(ac_mirobindir, MakeParams, yes)
			if test "x$ac_mirobindir" = xyes; then
				AC_SUBST(MIRO_MAKEPARAMS, "$MIRO_ROOT/src/makeParams/MakeParams")
			else
				AC_MSG_ERROR([Miro not (properly) installed. Source tarball and CVS at: http://smart.informatik.uni-ulm.de/MIRO/. Please check your installation! For more details about this problem, look at the end of config.log.])
			fi
		fi
	fi
	PATH=$OLD_PATH

	# Restore pretty messages.
	exec AC_FD_MSG>&8
	AC_MSG_RESULT($ac_mirobindir)
])
