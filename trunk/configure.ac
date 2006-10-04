## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/configure.ac
##
##############################################################################

AC_INIT(Miro, 2.1.0)
AC_CONFIG_AUX_DIR(config)
# AM_INIT_AUTOMAKE() # for newer automake versions
AM_INIT_AUTOMAKE(Miro, 2.1.0) # for older automake version
AM_CONFIG_HEADER(config.h)

AC_PREFIX_DEFAULT($PWD)

test ".$CFLAGS" = "." && CFLAGS=""
AC_PROG_CC
test ".$CFLAGS" = "." && test "$GCC" = "yes" && CFLAGS="-O2"

test ".$CXXFLAGS" = "." && CXXFLAGS=""
AC_PROG_CXX
test ".$CXXFLAGS" = "." && test "$GCC" = "yes" && CXXFLAGS="-O2"

AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_INSTALL

AC_DISABLE_STATIC
AC_ENABLE_SHARED
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h fcntl.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_WAIT3
AC_CHECK_FUNCS([alarm floor gethostname gethrtime gettimeofday inet_ntoa memmove memset munmap rint sqrt strcasecmp strerror strncasecmp])

# Checks for programs
AC_CHECK_PROG(ac_has_latex, latex, yes, no)
AC_CHECK_PROG(ac_has_dvips, dvips, yes, no)
AC_CHECK_PROG(ac_has_pdflatex, pdflatex, yes, no)
AC_CHECK_PROG(ac_has_convert, convert, yes, no)
AC_CHECK_PROG(ac_has_bibtex, bibtex, yes, no)
AC_CHECK_PROG(ac_has_makeindex, makeindex, yes, no)
AC_CHECK_PROG(ac_has_doxygen, doxygen, yes, no)
AC_CHECK_PROG(ac_has_fig2dev, fig2dev, yes, no)

AC_CHECK_LIB(termcap, free)

# our own specific paket tests

AC_SEARCH_QT
AC_SEARCH_ACE
AC_SEARCH_TAO
AC_FEATURES_COMPILE_ACE

AC_DETERMINE_LOGGING
AC_DETERMINE_DOCUMENTATION
AC_DETERMINE_SPEECH

AC_DETERMINE_PLATFORMS
AC_DETERMINE_VIDEODEVICES

AC_DETERMINE_GCC_VERSION
AC_DETERMINE_PCH

# now build the compiler flag lines used for derivative projects

miro_export_acetao_cflags="${ACE_CFLAGS} ${TAO_CFLAGS} ${INHERITED_CPPFLAGS} ${INHERITED_SYMBOLS} -W -Wall -Woverloaded-virtual -DDEBUG -DQT_GENUINE_STR"
miro_export_acetao_ldflags="${ACE_LDFLAGS} ${TAO_LDFLAGS}"
miro_export_acetao_libs="${ACE_LIBS} ${TAO_LIBS}"
miro_export_qt_cflags="${QT_CFLAGS}"
miro_export_qt_ldflags="${QT_LDFLAGS}"
miro_export_qt_libs="${QT_LIBS}"

if test "x$prefix" == xNONE; then
	if test "x$srcdir" != "x."; then
		miro_export_cflags="-I${PWD}/src -I${PWD} -I${PWD}/include -I${PWD}/${srcdir}/src -I${PWD}/${srcdir}";
		miro_export_ldflags="-L${PWD}/lib";
		miro_export_libs="-lmiroVideo -lmiroIDL -lmiro ${PWD}/lib/libmiroSvc.a";
	else
		miro_export_cflags="-I${PWD}/src -I${PWD} -I${PWD}/include"; 
		miro_export_ldflags="-L${PWD}/lib";
		miro_export_libs="-lmiroVideo -lmiroIDL -lmiro ${PWD}/lib/libmiroSvc.a";
	fi
else
	miro_export_cflags="-I${prefix}/src -I${prefix} -I${includedir}"; 
	miro_export_ldflags="-L${libdir}";
	miro_export_libs="-lmiroVideo -lmiroIDL -lmiro ${libdir}/libmiroSvc.a"; fi
if test "x$ac_miro_has_1394" == xyes; then if test $ac_miro_has_1394_libversion -ge 3; then miro_export_libs="$miro_export_libs -lraw1394 -ldc1394"; else miro_export_libs="$miro_export_libs -lraw1394 -ldc1394_control"; fi; fi
if test "x$ac_compile_player" == xyes; then miro_export_libs="$miro_export_libs ${PLAYER_LDFLAGS} ${PLAYER_LIBS}"; fi

AC_SUBST(miro_export_acetao_cflags)
AC_SUBST(miro_export_acetao_ldflags)
AC_SUBST(miro_export_acetao_libs)
AC_SUBST(miro_export_qt_cflags)
AC_SUBST(miro_export_qt_ldflags)
AC_SUBST(miro_export_qt_libs)
AC_SUBST(miro_export_cflags)
AC_SUBST(miro_export_ldflags)
AC_SUBST(miro_export_libs)

AC_SUBST(miro_export_makeparams, "${bindir}/MakeParams")
AC_SUBST(miro_export_acetao_idlcomp, "${TAO_IDL}")
AC_SUBST(miro_export_qt_moc, "${QT_MOC}")
AC_SUBST(miro_export_qt_uic, "${QT_UIC}")

AC_SUBST(miro_export_bttv, "$ac_miro_has_bttv")
AC_SUBST(miro_export_ieee1394, "$ac_miro_has_1394")
AC_SUBST(miro_export_meteor, "$ac_miro_has_meteor")
AC_SUBST(miro_export_quickcam, "$ac_miro_has_quickcam")
AC_SUBST(miro_export_sparrow99,"$ac_request_sparrow99")
AC_SUBST(miro_export_pioneer, "$ac_request_pioneer")
AC_SUBST(miro_export_b21, "$ac_request_b21")
AC_SUBST(miro_export_player, "$ac_compile_player")
AC_SUBST(miro_export_speech, "$ac_compile_speech")

# create makefiles

AC_CONFIG_FILES([Makefile
                 idl/Makefile
                 pch/Makefile
                 src/Makefile
                 src/params/Makefile
                 src/makeParams/Makefile
                 src/abus/Makefile
                 src/b21Base/Makefile
                 src/base/Makefile
                 src/buttons/Makefile
                 src/can/Makefile
                 src/faulMotor/Makefile
                 src/faulTty/Makefile
                 src/laser/Makefile
                 src/laser360/Makefile
		 src/fusedRangeSensor/Makefile
                 src/mcp/Makefile
                 src/miro/Makefile
                 src/msp/Makefile
                 src/panTilt/Makefile
                 src/pioneer/Makefile
                 src/pioneerBase/Makefile
                 src/psos/Makefile
                 src/sparrow/Makefile
                 src/sparrowBase/Makefile
                 src/speech/Makefile
                 src/sphinx/Makefile
                 src/video/Makefile
                 src/playerBase/Makefile
                 src/gps/Makefile
                 examples/Makefile
                 examples/b21Motion/Makefile
                 examples/battery/Makefile
                 examples/buttons/Makefile
                 examples/gps/Makefile
                 examples/infrared/Makefile
                 examples/motion/Makefile
                 examples/notify/Makefile
                 examples/odometry/Makefile
                 examples/params/Makefile
                 examples/rangeSensor/Makefile
                 examples/robotikI/Makefile
                 examples/sonar/Makefile
                 examples/speechSimple/Makefile
                 examples/stall/Makefile
                 examples/tcm2/Makefile
                 examples/video/Makefile
                 examples/videoFilter/Makefile
                 examples/videoFilter2/Makefile
                 performance-tests/Makefile
                 performance-tests/can/Makefile
                 performance-tests/mcp/Makefile
                 performance-tests/panTilt/Makefile
                 performance-tests/response/Makefile
                 performance-tests/video/Makefile
		 performance-tests/faul/Makefile
		 performance-tests/logging/Makefile
		 performance-tests/nmc/Makefile
                 tests/Makefile
		 tests/config/Makefile
                 tests/b21/Makefile
                 tests/dpPanTilt/Makefile
                 tests/dtlk/Makefile
                 tests/faul/Makefile
                 tests/gripper/Makefile
                 tests/motion/Makefile
                 tests/notify/Makefile
                 tests/notifyMulticast/Makefile
                 tests/odometry/Makefile
                 tests/panTilt/Makefile
                 tests/cameraControl/Makefile
                 tests/kicker/Makefile
                 tests/pioneer/Makefile
                 tests/rangeSensor/Makefile
		 tests/fusedRangeSensor/Makefile
                 tests/sparrow/Makefile
                 tests/speech/Makefile
                 tests/sphinx/Makefile
                 tests/video/Makefile
                 tests/videoBroker/Makefile
                 tests/log/Makefile
                 utils/Joystick/Makefile
                 utils/Makefile
                 utils/logPlayer/Makefile
                 utils/notify/Makefile
                 utils/nsIOR/Makefile
                 utils/configEditor/Makefile
                 utils/policyController/Makefile
                 utils/policyEditor/Makefile
                 utils/qtJoystick/Makefile
                 utils/qtTouchPad/Makefile
                 utils/qtVideo/Makefile
                 utils/rangeSensor/Makefile
                 utils/widgets/Makefile
		 utils/notifyObserver/Makefile
		 botsvcs/Makefile
		 botsvcs/BAP/Makefile
		 botsvcs/MCL/Makefile
		 botsvcs/tests/Makefile
                 botsvcs/tests/BAP/Makefile
                 botsvcs/tests/BAP/lib/Makefile
                 botsvcs/tests/BAP/parsing/Makefile
                 botsvcs/tests/BAP/engine/Makefile
                 botsvcs/tests/BAP/priorityArbiter/Makefile
                 botsvcs/tests/BAP/dynamicPolicy/Makefile
                 botsvcs/tests/BAP/monitor/Makefile
		 botsvcs/examples/Makefile
                 botsvcs/examples/BAP/Makefile
                 botsvcs/examples/BAP/b21/Makefile
                 botsvcs/examples/BAP/engine/Makefile
                 botsvcs/examples/BAP/rangeSensor/Makefile
                 botsvcs/examples/BAP/simple/Makefile
                 doc/Makefile
                 doc/fig/Makefile
                 doc/tex/Makefile
                 doc/html/Makefile
                 doc/html/doxygen.idl
                 doc/html/doxygen.cpp
                 dtd/Makefile
                 etc/Makefile
                 scripts/Makefile
                 bin/Makefile
                 lib/Makefile
                 bin/miro-config
])

AC_OUTPUT

# make miro-config executable so it is useable without installation
chmod a+x bin/miro-config



echo ""
echo "Miro configuration summary:"
echo ""
if test "x$ac_gcc_version" != "x"; then
	echo $ECHO_N "gcc version" $ac_gcc_version "is used."; fi
echo ""
echo $ECHO_N "Debug information "; if test "x$ac_request_debuginfo" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "Logging information "; if test "x$ac_request_loginfo" = xyes; then echo "enabled."; else echo "disabled."; fi
echo ""
echo "ACE-related results:"
echo "linker flags:  " $ACE_LDFLAGS
echo "compiler flags:" $ACE_CPPFLAGS
echo "libraries used:" $ACE_LIBS
echo ""
echo "TAO-related results:"
echo "linker flags:  " $TAO_LDFLAGS
echo "compiler flags:" $TAO_CPPFLAGS
echo "libraries used:" $TAO_LIBS
echo "used idl comp: " $TAO_IDL
echo ""
echo "inherited compilation values from ACE/TAO:"
echo "symbols:   " $ac_feature_acetao_symbols
echo "libraries: " $ac_feature_acetao_libs
echo "lib-dirs:  " $ac_feature_acetao_libdirs
echo "incl-dirs: " $ac_feature_acetao_incldirs
echo ""
echo "Qt-related results:"
echo "linker flags:  " $QT_LDFLAGS
echo "compiler flags:" $QT_CPPFLAGS
echo "libraries used:" $QT_LIBS
echo "used moc comp: " $QT_MOC
echo "used uic comp: " $QT_UIC
if test "x$ac_compile_speech" = xyes; then
	echo ""
	echo "Sphinx-related results:"
	echo "linker flags:  " $SPHINX_LDFLAGS
	echo "compiler flags:" $SPHINX_CPPFLAGS
	echo "libraries used:" $SPHINX_LIBS
	echo "Speechtools-related results:"
	echo "linker flags:  " $SPEECHTOOLS_LDFLAGS
	echo "compiler flags:" $SPEECHTOOLS_CPPFLAGS
	echo "libraries used:" $SPEECHTOOLS_LIBS
	echo "Festival-related results:"
	echo "linker flags:  " $FESTIVAL_LDFLAGS
	echo "compiler flags:" $FESTIVAL_CPPFLAGS
	echo "libraries used:" $FESTIVAL_LIBS
fi
if test x$ac_compile_player = xyes; then
	echo ""
	echo "Player-related results:"
	echo "linker flags:  " $PLAYER_LDFLAGS
	echo "compiler flags:" $PLAYER_CPPFLAGS
	echo "libraries used:" $PLAYER_LIBS
fi
echo ""
echo $ECHO_N "Sparrow99 support "; if test "x$ac_request_sparrow99" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "Pioneer support "; if test "x$ac_request_pioneer" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "B21 support "; if test "x$ac_request_b21" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "Player simulator support "; if test "x$ac_compile_player" = xyes; then echo "enabled."; else echo "disabled."; fi
echo ""
echo $ECHO_N "BTTV support "; if test "x$ac_miro_has_bttv" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "Meteor support "; if test "x$ac_miro_has_meteor" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "IEEE1394 support "; if test "x$ac_miro_has_1394" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "QuickCam support "; if test "x$ac_miro_has_quickcam" = xyes; then echo "enabled."; else echo "disabled."; fi
echo ""
echo $ECHO_N "Speech support "; if test "x$ac_compile_speech" = xyes; then echo "enabled."; else echo "disabled."; fi
echo ""
echo $ECHO_N "Latex documentation "; if test "x$ac_compile_docu" = xyes; then
	if test "x$ac_compile_pdf" = xyes; then echo "enabled using pdflatex."; else echo "enabled using latex and dvips."; fi
	else echo "disabled."; fi
echo $ECHO_N "HTML documentation "; if test "x$ac_compile_html" = xyes; then echo "enabled."; else echo "disabled."; fi
echo ""
