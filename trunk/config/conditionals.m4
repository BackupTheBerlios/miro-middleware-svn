## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/config/conditionals.m4
##
##############################################################################


## platforms

AC_DEFUN([AC_DETERMINE_PLATFORMS],
[
	AC_ARG_ENABLE(
		B21,
		AC_HELP_STRING([--enable-B21], [Turn on B21 support (default off)]),
		ac_request_b21=$enableval,
		ac_request_b21=no)

	AC_ARG_ENABLE(
		Pioneer,
		AC_HELP_STRING([--enable-Pioneer], [Turn on Pioneer support (default on)]),
		ac_request_pioneer=$enableval,
		ac_request_pioneer=yes)

	AC_ARG_ENABLE(
		Sparrow99,
		AC_HELP_STRING([--enable-Sparrow99], [Turn on Sparrow99 support (default on)]),
		ac_request_sparrow99=$enableval,
		ac_request_sparrow99=yes)

	AC_ARG_ENABLE(
		Player,
		AC_HELP_STRING([--enable-Player], [Turn on Player simulator support (default off)]),
		ac_request_player=$enableval,
		ac_request_player=no)

	AC_ARG_ENABLE(
		Deprecated,
		AC_HELP_STRING([--enable-Deprecated], [Turn on support for deprecated interfaces]),
		ac_request_deprecated=$enableval,
		ac_request_deprecated=no)
])



## video devices

AC_DEFUN([AC_DETERMINE_VIDEODEVICES],
[
AC_ARG_ENABLE(
	BTTV,
	AC_HELP_STRING([--enable-BTTV], [Turn on BTTV support (default on)]),
	ac_request_bttv=$enableval,
	ac_request_bttv=yes)

AC_ARG_ENABLE(
	Meteor,
	AC_HELP_STRING([--enable-Meteor], [Turn on Meteor support (default off)]),
	ac_request_meteor=$enableval,
	ac_request_meteor=no)

AC_ARG_ENABLE(
	IEEE1394,
	AC_HELP_STRING([--enable-IEEE1394], [Turn on IEEE1394 support (default on)]),
	ac_request_ieee1394=$enableval,
	ac_request_ieee1394=yes)

AC_ARG_ENABLE(
	QuickCam,
	AC_HELP_STRING([--enable-QuickCam], [Turn on QuickCam support (default on)]),
	ac_request_quickcam=$enableval,
	ac_request_quickcam=yes)
])



## misc additional stuff

AC_DEFUN([AC_DETERMINE_ADDITIONAL],
[
	AC_ARG_ENABLE(
		Documentation,
		AC_HELP_STRING([--enable-Documentation], [Build the documentation (default off)]),
		ac_request_docu=$enableval,
		ac_request_docu=no)

	AC_ARG_ENABLE(
		Speech,
		AC_HELP_STRING([--enable-Speech], [Turn on Speech support (default off)]),
		ac_request_speech=$enableval,
		ac_request_speech=no)
])


## check gcc version

AC_DEFUN([AC_DETERMINE_GCC_VERSION],
[
	if test "x$GCC" = "xyes"; then
		dnl cut out the version number (additional (blank) lines are eliminated)
		dnl ac_gcc_version=`for i in \`$CC --version\`; do expr $i : '\([[0-9]]\+\(\.[[0-9]]\+\)\+\)'; done | grep -v ^$`
		ac_gcc_version=`$CC -dumpversion`
		ac_gcc_major=`echo $ac_gcc_version | cut -d . -f 1 -`
		ac_gcc_minor=`echo $ac_gcc_version | cut -d . -f 2 -`
	else
		ac_gcc_version=
	fi

	AH_TEMPLATE([GCC_VERSION], [the used gcc version.])
	AH_TEMPLATE([GCC_MAJOR_VERSION], [the major number of the used gcc version.])
	AH_TEMPLATE([GCC_MINOR_VERSION], [the minor number of the used gcc version.])

	if test "x$ac_gcc_version" != "x"; then
		AC_DEFINE_UNQUOTED(GCC_VERSION, $ac_gcc_version)
		AC_DEFINE_UNQUOTED(GCC_MAJOR_VERSION, $ac_gcc_major)
		AC_DEFINE_UNQUOTED(GCC_MINOR_VERSION, $ac_gcc_minor)
	fi
])


## check for precompiled headers

AC_DEFUN([AC_DETERMINE_PCH],
[
	AC_ARG_ENABLE(
		PCH,
		AC_HELP_STRING([--enable-PCH], [Turn on support for precompiled header (default off)]),
		ac_request_pch=$enableval,
		ac_request_pch=no)


	AH_TEMPLATE([MIRO_HAS_PCH], [build with support for precompiled header.])
	if test "x$ac_request_pch" = xyes; then
		AC_MSG_CHECKING(PCH support)
		if test $ac_gcc_major -ge 4 || (test $ac_gcc_major -ge 3 && test $ac_gcc_minor -ge 4); then
			AC_DEFINE(MIRO_HAS_PCH)
			ac_use_pch=yes
			AC_MSG_RESULT(yes)
			AC_MSG_CHECKING(create single PCH header (may take some time))

			if test -f pch/all.h; then
				AC_MSG_NOTICE([Header file allready exists - skipping this. If you want to rebuild the header file, simply remove the file $MIRO_ROOT/pch/all.h and rerun the configure script or call the script $MIRO_ROOT/all.h.sh manually.])
			else
# write config option to file, so they can be used later on
				echo $ACE_CFLAGS $TAO_CFLAGS $QT_CFLAGS $INHERITED_CPPFLAGS $INHERITED_SYMBOLS -W -Wall -Woverloaded-virtual -DDEBUG -DQT_GENUINE_STR $CXXFLAGS $AM_CXXFLAGS -DHAVE_CONFIG_H > ./all.h.in
				./all.h.sh
				if test $? == 0; then
					AC_MSG_RESULT(yes)
				else
					AC_MSG_RESULT(no)
				fi
			fi
		else
			AC_MSG_RESULT(no)
			AC_MSG_WARN([PCH requested but compiler too old.])
		fi
	fi
	AM_CONDITIONAL(COND_PCH, [test "x$ac_use_pch" = xyes])
])



## logging and debuffing related stuff

AC_DEFUN([AC_DETERMINE_LOGGING],
[
	AC_ARG_ENABLE(
		DebugInfo,
		AC_HELP_STRING([--enable-DebugInfo], [Enable Miro debug information (default on)]),
		ac_request_debuginfo=$enableval,
		ac_request_debuginfo=yes)

	AC_ARG_ENABLE(
		LogInfo,
		AC_HELP_STRING([--enable-LogInfo], [Enable Miro logging information (default on)]),
		ac_request_loginfo=$enableval,
		ac_request_loginfo=yes)
])



## check libdc version

AC_DEFUN([AC_DETERMINE_LIBDC_VERSION],
[
	AC_LANG_PUSH(C)
	AC_MSG_CHECKING(how to setup dma capture)
	success=failed
	AC_TRY_COMPILE([
		#include <libdc1394/dc1394_control.h>
	],[
		raw1394handle_t handle;
		nodeid_t node;
		dc1394_dma_setup_capture(
			handle,
			node,
			1,
			FORMAT_VGA_NONCOMPRESSED,
			MODE_640x480_YUV422,
			SPEED_400,
			FRAMERATE_30, 
			4,
			1,
			1,
			"/dev/blabla",
			(dc1394_cameracapture *)NULL);
	],[
	success=2
	],[
	])
	AC_TRY_COMPILE([
		#include <libdc1394/dc1394_control.h>
	],[
		raw1394handle_t handle;
		nodeid_t node;
		dc1394_dma_setup_capture(
			handle,
			node,
			1,
			FORMAT_VGA_NONCOMPRESSED,
			MODE_640x480_YUV422,
			SPEED_400,
			FRAMERATE_30, 
			4,
			1,
			"/dev/blabla",
			(dc1394_cameracapture *)NULL);
	],[
	success=1
	],[
	])
	AC_MSG_RESULT($success)
	AC_LANG_POP()

	AH_TEMPLATE([LIBDC1394_VERSION], [the used libdc1394 version.])
	if test "x$success" != "xfailed"; then
		AC_DEFINE_UNQUOTED(LIBDC1394_VERSION, $success)
	else
		AC_MSG_ERROR([Cannot determine libdc1394 version. Giving up. For more details about this problem, look at the end of config.log.])
	fi
])
