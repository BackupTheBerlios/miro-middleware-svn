## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/video/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))


## dynamic libraries if needed
LDLIBS += -lmiro -lraw1394 -ldc1394_control

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= VideoService

## additional files
FILES = \
	Parameters \
	VideoConfig \
	VideoDevice \
	VideoDeviceBase \
	VideoDeviceBTTV \
	VideoDeviceMeteor \
	VideoDevice1394 \
	VideoDeviceDummy \
	VideoConnection \
	VideoConsumer \
	VideoImpl \
	VideoConverter \
	VideoConverterYUV411toRGB \
	VideoConverterYUV411toYUV \
	VideoConverterYUV422toRGB \
	VideoConverterYUV422toYUV \
	VideoConverterYUVbase \
	VideoServer \
	VideoService

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

$(BIN): $(STATIC_DEPS)

.PRECIOUS: Parameters.cpp Parameters.h

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.so .shobj/Parameters.o .shobj/Parameters.so: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/VideoConfig.o .obj/VideoConfig.so .shobj/VideoConfig.o .shobj/VideoConfig.so: VideoConfig.cpp VideoConfig.h

.obj/VideoDevice.o .obj/VideoDevice.so .shobj/VideoDevice.o .shobj/VideoDevice.so: VideoDevice.cpp VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 VideoDeviceBase.h VideoDeviceBTTV.h VideoDeviceMeteor.h \
 ioctl_meteor.h VideoDevice1394.h VideoDeviceDummy.h

.obj/VideoDeviceBase.o .obj/VideoDeviceBase.so .shobj/VideoDeviceBase.o .shobj/VideoDeviceBase.so: VideoDeviceBase.cpp VideoDeviceBase.h VideoDevice.h \
 Parameters.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/VideoDeviceBTTV.o .obj/VideoDeviceBTTV.so .shobj/VideoDeviceBTTV.o .shobj/VideoDeviceBTTV.so: VideoDeviceBTTV.cpp VideoDeviceBTTV.h \
 VideoDeviceBase.h VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/VideoDeviceMeteor.o .obj/VideoDeviceMeteor.so .shobj/VideoDeviceMeteor.o .shobj/VideoDeviceMeteor.so: VideoDeviceMeteor.cpp VideoDeviceMeteor.h \
 ioctl_meteor.h VideoDeviceBase.h VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/VideoDevice1394.o .obj/VideoDevice1394.so .shobj/VideoDevice1394.o .shobj/VideoDevice1394.so: VideoDevice1394.cpp VideoDevice1394.h \
 VideoDeviceBase.h VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/VideoDeviceDummy.o .obj/VideoDeviceDummy.so .shobj/VideoDeviceDummy.o .shobj/VideoDeviceDummy.so: VideoDeviceDummy.cpp VideoDeviceDummy.h \
 VideoDeviceBase.h VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/VideoConnection.o .obj/VideoConnection.so .shobj/VideoConnection.o .shobj/VideoConnection.so: VideoConnection.cpp VideoConnection.h VideoDevice.h \
 Parameters.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 VideoConsumer.h $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/VideoConsumer.o .obj/VideoConsumer.so .shobj/VideoConsumer.o .shobj/VideoConsumer.so: VideoConsumer.cpp VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i VideoConnection.h \
 VideoDevice.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 VideoImpl.h $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/VideoC.h \
 $(MIRO_ROOT)/src/miro/VideoC.i \
 $(MIRO_ROOT)/src/miro/VideoS.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

.obj/VideoImpl.o .obj/VideoImpl.so .shobj/VideoImpl.o .shobj/VideoImpl.so: VideoImpl.cpp VideoImpl.h VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/VideoC.h \
 $(MIRO_ROOT)/src/miro/VideoC.i \
 $(MIRO_ROOT)/src/miro/VideoS.i VideoDevice.h

.obj/VideoConverter.o .obj/VideoConverter.so .shobj/VideoConverter.o .shobj/VideoConverter.so: VideoConverter.cpp VideoConverter.h

.obj/VideoConverterYUV411toRGB.o .obj/VideoConverterYUV411toRGB.so .shobj/VideoConverterYUV411toRGB.o .shobj/VideoConverterYUV411toRGB.so: VideoConverterYUV411toRGB.cpp \
 VideoConverterYUV411toRGB.h VideoConverterYUVbase.h VideoConverter.h

.obj/VideoConverterYUV411toYUV.o .obj/VideoConverterYUV411toYUV.so .shobj/VideoConverterYUV411toYUV.o .shobj/VideoConverterYUV411toYUV.so: VideoConverterYUV411toYUV.cpp \
 VideoConverterYUV411toYUV.h VideoConverter.h

.obj/VideoConverterYUV422toRGB.o .obj/VideoConverterYUV422toRGB.so .shobj/VideoConverterYUV422toRGB.o .shobj/VideoConverterYUV422toRGB.so: VideoConverterYUV422toRGB.cpp \
 VideoConverterYUV422toRGB.h VideoConverterYUVbase.h VideoConverter.h

.obj/VideoConverterYUV422toYUV.o .obj/VideoConverterYUV422toYUV.so .shobj/VideoConverterYUV422toYUV.o .shobj/VideoConverterYUV422toYUV.so: VideoConverterYUV422toYUV.cpp \
 VideoConverterYUV422toYUV.h VideoConverter.h

.obj/VideoConverterYUVbase.o .obj/VideoConverterYUVbase.so .shobj/VideoConverterYUVbase.o .shobj/VideoConverterYUVbase.so: VideoConverterYUVbase.cpp \
 VideoConverterYUVbase.h VideoConverter.h

.obj/VideoServer.o .obj/VideoServer.so .shobj/VideoServer.o .shobj/VideoServer.so: VideoServer.cpp VideoServer.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 VideoConnection.h \
 VideoDevice.h \
 Parameters.h \
 VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 VideoImpl.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/VideoC.h \
 $(MIRO_ROOT)/src/miro/VideoC.i \
 $(MIRO_ROOT)/src/miro/VideoS.i Parameters.h \
 $(MIRO_ROOT)/src/miro/Utils.h

.obj/VideoService.o .obj/VideoService.so .shobj/VideoService.o .shobj/VideoService.so: VideoService.cpp VideoServer.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 VideoConnection.h \
 VideoDevice.h \
 Parameters.h \
 VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 VideoImpl.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/VideoC.h \
 $(MIRO_ROOT)/src/miro/VideoC.i \
 $(MIRO_ROOT)/src/miro/VideoS.i \
 $(MIRO_ROOT)/src/miro/Utils.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
