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
			TAO_ROOT="$withval"
		fi,
		ac_with_TAO=no
	)

# look where TAO is installed
	AC_SEARCH_TAO_POSITION()
# which libraries do we need
	AC_SEARCH_TAO_LIBRARIES()
# search the idl compiler
	AC_SEARCH_TAO_IDL()
])



AC_DEFUN([AC_SEARCH_TAO_POSITION],
[
	ac_search_tao_success=no
	AC_MSG_CHECKING(for TAO)

	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

# let's try some different possible places
# if there is a directory given at the command line or if there is a environment variable
# called TAO_ROOT, use this one
	if test "${TAO_ROOT}" != "" ; then
		LDFLAGS="$LDFLAGS -L$ACE_ROOT/ace -lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies"
		CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -I$TAO_ROOT -I$TAO_ROOT/orbsvcs -D_GNU_SOURCE"
		CFLAGS="$CFLAGS $ACE_CFLAGS -I$TAO_ROOT -I$TAO_ROOT/orbsvcs"
		AC_SEARCH_TAO_LOCAL()
		AC_SUBST(TAO_ROOT, "$TAO_ROOT")
		AC_SUBST(TAO_LDFLAGS, "-L$ACE_ROOT/ace")
		AC_SUBST(TAO_CPPFLAGS, "-I$TAO_ROOT -I$TAO_ROOT/orbsvcs -D_GNU_SOURCE")
		AC_SUBST(TAO_CFLAGS, "-I$TAO_ROOT -I$TAO_ROOT/orbsvcs")
		AC_SUBST(TAO_LIBS, "-lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies")
	else
#otherwise try without any additional search directories
		CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -D_GNU_SOURCE"
		CFLAGS="$CFLAGS $ACE_CFLAGS -D_GNU_SOURCE"
		LDFLAGS="$LDFLAGS -lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies"
		AC_SEARCH_TAO_LOCAL()
		AC_SUBST(TAO_LIBS, "-lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies")
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

# if we did not find TAO, assume a never version, where the TAO libs are installed in /lib instead of /ace
	if test x$ac_search_tao_success == "xno" ; then
		if test "${TAO_ROOT}" != "" ; then
			LDFLAGS="$LDFLAGS -L$ACE_ROOT/lib -lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies"
			CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -I$TAO_ROOT -I$TAO_ROOT/orbsvcs -D_GNU_SOURCE"
			CFLAGS="$CFLAGS $ACE_CFLAGS -I$TAO_ROOT -I$TAO_ROOT/orbsvcs"
			AC_SEARCH_TAO_LOCAL()
			AC_SUBST(TAO_ROOT, "$TAO_ROOT")
			AC_SUBST(TAO_LDFLAGS, "-L$ACE_ROOT/lib")
			AC_SUBST(TAO_CPPFLAGS, "-I$TAO_ROOT -I$TAO_ROOT/orbsvcs -D_GNU_SOURCE")
			AC_SUBST(TAO_CFLAGS, "-I$TAO_ROOT -I$TAO_ROOT/orbsvcs")
			AC_SUBST(TAO_LIBS, "-lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies")
		fi
	fi

# if there is still no TAO found, try all these places but assume, that TAO is actually
# installed (using make install, which is available for never versions)
	if test x$ac_search_tao_success == "xno" ; then
		if test "${TAO_ROOT}" != "" ; then
			LDFLAGS="$LDFLAGS -L$TAO_ROOT/lib -lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies"
			CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS -I$TAO_ROOT/include -D_GNU_SOURCE"
			CFLAGS="$CFLAGS $ACE_CFLAGS -I$TAO_ROOT/include"
			AC_SEARCH_TAO_LOCAL()
			AC_SUBST(TAO_ROOT, "$TAO_ROOT")
			AC_SUBST(TAO_LDFLAGS, "-L$TAO_ROOT/lib")
			AC_SUBST(TAO_CPPFLAGS, "-I$TAO_ROOT/include -D_GNU_SOURCE")
			AC_SUBST(TAO_CFLAGS, "-I$TAO_ROOT/include")
			AC_SUBST(TAO_LIBS, "-lTAO -lTAO_CosNotification -lTAO_CosNaming -lTAO_Strategies")
		fi
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

# if there is still no TAO found, emit an error message and stop
	AC_MSG_RESULT($ac_search_tao_success)
	if test x$ac_search_tao_success == "xno"; then
		AC_MSG_ERROR([TAO not (properly) installed. Source tarball and CVS at: http://deuce.doc.wustl.edu/Download.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi
])


AC_DEFUN([AC_SEARCH_TAO_LIBRARIES],
[
	AC_MSG_CHECKING(for TAO libraries)

	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

# static part of the flags
	CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS $TAO_CPPFLAGS -D_GNU_SOURCE"
	CFLAGS="$CFLAGS $ACE_CFLAGS $TAO_CPPFLAGS"

# variable part
	ac_search_tao_success=no
	LDFLAGS="$LDFLAGS $TAO_LDFLAGS $TAO_LIBS -lTAO_PortableServer -lTAO_CosNotification_Skel -lTAO_CosNotification_Serv"
	AC_SEARCH_TAO_LOCAL()
	if test x$ac_search_tao_success != "xno" ; then
		AC_SUBST(TAO_LIBS, "$TAO_LIBS -lTAO_PortableServer -lTAO_CosNotification_Skel -lTAO_CosNotification_Serv")
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS

	AC_MSG_RESULT(yes)
])




AC_DEFUN([AC_SEARCH_TAO_IDL],
[
	AC_MSG_CHECKING([for the TAO IDL compiler])

# Save the "AC_MSG_RESULT file descriptor" to FD 8.
	exec 8>&AC_FD_MSG
# Temporarily turn off AC_MSG_RESULT so that the user gets pretty messages.
	exec AC_FD_MSG>/dev/null

# search in the standard path or the path defined by $TAO_ROOT
	if test "${TAO_ROOT}" == "" ; then
		AC_PATH_PROG(ac_taobindir, tao_idl, no)
	else
		AC_PATH_PROG(ac_taobindir, tao_idl, no, [$TAO_ROOT/TAO_IDL:$PATH])
	fi

# still not found? try the path of an installed tao version
	if test x$ac_taobindir == xno ; then
# argl, make sure that this test use another variable, otherwise this test uses the cached
# result of the previous test and is therefor always false also
		AC_PATH_PROG(ac_taobindir2, tao_idl, no, [$TAO_ROOT/bin])
	fi

## Restore pretty messages.
	exec AC_FD_MSG>&8

	if test x$ac_taobindir != xno || test x$ac_taobindir2 != xno ; then
		if test x$ac_taobindir != xno; then
			AC_SUBST(TAO_IDL, "$ac_taobindir")
		else
			AC_SUBST(TAO_IDL, "$ac_taobindir2")
		fi
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([TAO not (properly) installed. Source tarball and CVS at: http://deuce.doc.wustl.edu/Download.html. Please check your installation! For more details about this problem, look at the end of config.log.])
	fi
	AC_MSG_RESULT(yes)
])




AC_DEFUN([AC_SEARCH_TAO_LOCAL],
[
	AC_LANG_PUSH(C++)
	AC_TRY_LINK([
		#include <tao/ORB.h>
	],[
		;
	],[
	success=yes
	],[
	success=no
	])
	AC_LANG_POP()
	ac_search_tao_success=$success
])
