## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/configure.ac
##
##############################################################################

AC_INIT(Miro, 2.0.0)
AC_CONFIG_AUX_DIR(config)
# AM_INIT_AUTOMAKE() # for never automake versions
AM_INIT_AUTOMAKE(Miro, 2.0.0) 

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

# Checks for libraries.
AC_CHECK_LIB(raw1394, raw1394_get_libversion, [ac_have_libraw1394=yes], [ac_have_libraw1394=no])
AC_CHECK_LIB(dc1394_control, dc1394_print_feature_set, [ac_have_libdc1394=yes], [ac_have_libdc1394=no], [-lraw1394])

AC_DETERMINE_LIBDC_VERSION

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

AC_DETERMINE_ADDITIONAL

# our own specific paket tests
AC_CHECK_LIB(termcap, free)
if test "x$ac_request_speech" = xyes; then
	AC_SEARCH_SPEECHTOOLS
	AC_SEARCH_SPHINX
	AC_SEARCH_FESTIVAL
fi
AC_SEARCH_QT
AC_SEARCH_ACE
AC_SEARCH_TAO
AC_FEATURES_COMPILE_ACE

# conditional compilation

AC_DETERMINE_PLATFORMS

AM_CONDITIONAL(COND_SPARROW99, [test "x$ac_request_sparrow99" = xyes])
AM_CONDITIONAL(COND_PIONEER,   [test "x$ac_request_pioneer" = xyes])
AM_CONDITIONAL(COND_B21,       [test "x$ac_request_b21" = xyes])
AM_CONDITIONAL(COND_DEPRECATED,[test "x$ac_request_deprecated" = xyes])

AC_DETERMINE_VIDEODEVICES

AH_TEMPLATE([MIRO_HAS_BTTV], [build with 1394 support.])
AH_TEMPLATE([MIRO_HAS_METEOR], [build with Meteor support.])
AH_TEMPLATE([MIRO_HAS_1394], [build with BTTV support.])
AH_TEMPLATE([MIRO_HAS_QUICKCAM], [build with QuickCam support.])

if test "x$ac_request_bttv" = xyes; then
	AC_DEFINE(MIRO_HAS_BTTV)
	ac_miro_has_bttv=yes
fi

if test "x$ac_request_meteor" = xyes; then
	AC_DEFINE(MIRO_HAS_METEOR)
	ac_miro_has_meteor=yes
fi

if test "x$ac_request_ieee1394" = xyes &&
	test "x$ac_have_libraw1394" = xyes &&
	test "x$ac_have_libdc1394" = xyes; then
	AC_DEFINE(MIRO_HAS_1394)
	ac_miro_has_1394=yes
fi

if test "x$ac_request_quickcam" = xyes; then
	AC_DEFINE(MIRO_HAS_QUICKCAM)
	ac_miro_has_quickcam=yes
fi

AM_CONDITIONAL(COND_BTTV, [test "x$ac_miro_has_bttv" = xyes])
AM_CONDITIONAL(COND_METEOR, [test "x$ac_miro_has_meteor" = xyes])
AM_CONDITIONAL(COND_IEEE1394, [test "x$ac_miro_has_1394" = xyes])
AM_CONDITIONAL(COND_QUICKCAM, [test "x$ac_miro_has_quickcam" = xyes])

if test "x$ac_request_docu" = xyes; then
	if test "x$ac_has_bibtex" = xyes &&
	test "x$ac_has_makeindex" = xyes &&
	test "x$ac_has_convert" = xyes &&
	test "x$ac_has_fig2dev" = xyes &&
	 ((test "x$ac_has_latex" = xyes &&
	   test "x$ac_has_dvips" = xyes) ||
	  (test "x$ac_has_pdflatex" = xyes)); then
		ac_compile_docu=yes
	fi

	if test "x$ac_has_pdflatex" = xyes &&
	test "x$ac_has_fig2dev" = xyes &&
	test "x$ac_has_convert" = xyes; then
		ac_compile_pdf=yes
	fi

	if test "x$ac_has_doxygen" = xyes; then
		ac_compile_html=yes
	fi
fi

AM_CONDITIONAL(COND_TEXDOC, [test "x$ac_compile_docu" = xyes])
AM_CONDITIONAL(COND_PDF, [test "x$ac_compile_pdf" = xyes])
AM_CONDITIONAL(COND_HTMLDOC, [test "x$ac_compile_html" = xyes])

if test "x$ac_request_speech" = xyes &&
	test "x$ac_has_sphinx" = xyes &&
	test "x$ac_has_festival" = xyes &&
	test "x$ac_has_speechtools" = xyes; then
	ac_compile_speech=yes
fi

AM_CONDITIONAL(COND_SPEECH, [test "x$ac_compile_speech" = xyes])



AH_TEMPLATE([MIRO_NO_DEBUG], [build without debug info support.])
AH_TEMPLATE([MIRO_NO_LOGGING], [build without log info support.])

AC_DETERMINE_LOGGING
if test "x$ac_request_debuginfo" = xno; then
	AC_DEFINE(MIRO_NO_DEBUG)
fi
if test "x$ac_request_loginfo" = xno; then
	AC_DEFINE(MIRO_NO_LOGGING)
fi


AC_DETERMINE_GCC_VERSION

AC_CONFIG_FILES([Makefile
                 idl/Makefile
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
                 src/image/Makefile
                 src/laser/Makefile
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
                 examples/Makefile
                 examples/b21Motion/Makefile
                 examples/battery/Makefile
                 examples/buttons/Makefile
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
                 examples/video/Makefile
                 examples/videoFilter/Makefile
                 examples/behaviours/Makefile
                 examples/behaviours/b21/Makefile
                 examples/behaviours/engine/Makefile
                 examples/behaviours/rangeSensor/Makefile
                 examples/behaviours/simple/Makefile
                 performance-tests/Makefile
                 performance-tests/can/Makefile
                 performance-tests/image/Makefile
                 performance-tests/mcp/Makefile
                 performance-tests/panTilt/Makefile
                 performance-tests/response/Makefile
                 performance-tests/video/Makefile
		 performance-tests/faul/Makefile
                 tests/Makefile
                 tests/b21/Makefile
                 tests/behaviourEngine/Makefile
                 tests/behaviours/Makefile
                 tests/dpPanTilt/Makefile
                 tests/dtlk/Makefile
                 tests/faul/Makefile
                 tests/gripper/Makefile
                 tests/image/Makefile
                 tests/motion/Makefile
                 tests/notify/Makefile
                 tests/notifyMulticast/Makefile
                 tests/odometry/Makefile
                 tests/panTilt/Makefile
                 tests/pioneer/Makefile
                 tests/rangeSensor/Makefile
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
                 utils/policyController/Makefile
                 utils/policyEditor/Makefile
                 utils/qtJoystick/Makefile
                 utils/qtTouchPad/Makefile
                 utils/qtVideo/Makefile
                 utils/rangeSensor/Makefile
                 utils/widgets/Makefile
		 utils/notifyObserver/Makefile
		 utils/makeConfig/Makefile
                 doc/Makefile
                 doc/fig/Makefile
                 doc/tex/Makefile
                 doc/html/Makefile
                 dtd/Makefile
                 etc/Makefile
                 scripts/Makefile
                 bin/Makefile
                 lib/Makefile
])

#                 tests/xml/Makefile
#                 utils/makeConfig/Makefile
#                 utils/sparrowBoard/Makefile
#                 miro-2.0.0.pc

AC_OUTPUT



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
# echo "warnings:  " $ac_feature_acetao_warnings
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
echo ""
echo $ECHO_N "Sparrow99 support "; if test "x$ac_request_sparrow99" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "Pioneer support "; if test "x$ac_request_pioneer" = xyes; then echo "enabled."; else echo "disabled."; fi
echo $ECHO_N "B21 support "; if test "x$ac_request_b21" = xyes; then echo "enabled."; else echo "disabled."; fi
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



# AC_SUBST(MIRO_ROOT, "$PWD")
