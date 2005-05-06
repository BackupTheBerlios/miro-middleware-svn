AC_DEFUN(AC_FEATURES_SPARROW_MIRO,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	LDFLAGS="$LDFLAGS $ACE_LDFLAGS $TAO_LDFLAGS $MIRO_LDFLAGS $QT_LDFLAGS -lsparrow -lcan -lmiroIDL -lmiro -lmiroSvc"
	CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS $TAO_CPPFLAGS $QT_CPPFLAGS $MIRO_CPPFLAGS"
	CFLAGS="$CFLAGS $ACE_CFLAGS $TAO_CFLAGS $QT_CFLAGS $MIRO_CFLAGS"

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for Sparrow support in Miro)
	AC_TRY_LINK([
		#include <idl/KickerC.h>
	],[
		;
	],[
	success=yes
	],[
	success=no
	])
	AC_MSG_RESULT($success)
	AC_LANG_POP()

	ac_miro_compiled_with_sparrow99=$success
	if test "x$success" != xyes; then
		AC_MSG_ERROR(Miro not complied with Sparrow99 support.)
	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS
])



AC_DEFUN(AC_FEATURES_BTTV_MIRO,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	LDFLAGS="$LDFLAGS $ACE_LDFLAGS $TAO_LDFLAGS $MIRO_LDFLAGS $QT_LDFLAGS -lmiroVideo -lmiroIDL -lmiro -lmiroSvc"
	CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS $TAO_CPPFLAGS $QT_CPPFLAGS -I$MIRO_ROOT/src/video $MIRO_CPPFLAGS"
	CFLAGS="$CFLAGS $ACE_CFLAGS $TAO_CFLAGS $QT_CFLAGS -I$MIRO_ROOT/src/video $MIRO_CFLAGS"

	if test "x$ac_have_libraw1394" = xyes; then
		LDFLAGS="$LDFLAGS -lraw1394"
	fi
	if test "x$ac_have_libdc1394" = xyes; then
		LDFLAGS="$LDFLAGS -ldc1394_control"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for BTTV support in Miro)
	AC_TRY_LINK([
#include "VideoServer.h"
#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "video/Parameters.h"
#include "video/VideoDevice.h"
#include <video/VideoDeviceBTTV.h>
	],[
Miro::ImageFormatIDL format;
Video::DeviceBTTV *device = new Video::DeviceBTTV(format); 
	],[
	success=yes
	],[
	success=no
	])
	AC_MSG_RESULT($success)
	AC_LANG_POP()

	ac_miro_compiled_with_bttv=$success
#	if test "x$success" != xyes; then
#		AC_MSG_ERROR(Miro not complied with BTTV support.)
#	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS
])



AC_DEFUN(AC_FEATURES_IEEE1394_MIRO,
[
	OLD_LDFLAGS=$LDFLAGS
	OLD_CFLAGS=$CFLAGS
	OLD_CPPFLAGS=$CPPFLAGS

	LDFLAGS="$LDFLAGS $ACE_LDFLAGS $TAO_LDFLAGS $MIRO_LDFLAGS $QT_LDFLAGS -lmiroVideo -lmiroIDL -lmiro -lmiroSvc"
	CPPFLAGS="$CPPFLAGS $ACE_CPPFLAGS $TAO_CPPFLAGS $QT_CPPFLAGS -I$MIRO_ROOT/src/video $MIRO_CPPFLAGS"
	CFLAGS="$CFLAGS $ACE_CFLAGS $TAO_CFLAGS $QT_CFLAGS -I$MIRO_ROOT/src/video $MIRO_CFLAGS"

	if test "x$ac_have_libraw1394" = xyes; then
		LDFLAGS="$LDFLAGS -lraw1394"
	fi
	if test "x$ac_have_libdc1394" = xyes; then
		LDFLAGS="$LDFLAGS -ldc1394_control"
	fi

	AC_LANG_PUSH(C++)
	AC_MSG_CHECKING(for IEEE1394 support in Miro)
	AC_TRY_LINK([
#include "VideoServer.h"
#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "video/Parameters.h"
#include "video/VideoDevice.h"
#include <video/VideoDevice1394.h>
	],[
Miro::ImageFormatIDL format;
Video::Device1394 *device = new Video::Device1394(format);
	],[
	success=yes
	],[
	success=no
	])
	AC_MSG_RESULT($success)
	AC_LANG_POP()

	ac_miro_compiled_with_ieee1394=$success
#	if test "x$success" != xyes; then
#		AC_MSG_ERROR(Miro not complied with IEEE1394 support.)
#	fi

	LDFLAGS=$OLD_LDFLAGS
	CPPFLAGS=$OLD_CPPFLAGS
	CFLAGS=$OLD_CFLAGS
])
