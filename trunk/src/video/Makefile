## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
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
LDLIBS += -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= VideoService

## additional files
FILES = \
	VideoDevice \
	VideoDeviceBTTV \
	VideoDeviceMeteor \
	VideoConnection \
	VideoConsumer \
	VideoParameters \
	VideoImpl \
	VideoServer

##	videoDeviceV4L \
##	videoDevice1394 \

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

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/VideoDevice.o .obj/VideoDevice.so .shobj/VideoDevice.o .shobj/VideoDevice.so: VideoDevice.cpp VideoDevice.h VideoDeviceBTTV.h \
 VideoDeviceMeteor.h ioctl_meteor.h

.obj/VideoDeviceBTTV.o .obj/VideoDeviceBTTV.so .shobj/VideoDeviceBTTV.o .shobj/VideoDeviceBTTV.so: VideoDeviceBTTV.cpp VideoDeviceBTTV.h VideoDevice.h

.obj/VideoDeviceMeteor.o .obj/VideoDeviceMeteor.so .shobj/VideoDeviceMeteor.o .shobj/VideoDeviceMeteor.so: VideoDeviceMeteor.cpp VideoDeviceMeteor.h \
 ioctl_meteor.h VideoDevice.h

.obj/VideoConnection.o .obj/VideoConnection.so .shobj/VideoConnection.o .shobj/VideoConnection.so: VideoConnection.cpp VideoConnection.h VideoDevice.h \
 VideoConsumer.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Task.h VideoParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/VideoConsumer.o .obj/VideoConsumer.so .shobj/VideoConsumer.o .shobj/VideoConsumer.so: VideoConsumer.cpp VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Task.h VideoConnection.h VideoDevice.h \
 VideoImpl.h VideoConsumer.h \
 VideoParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
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
 $(MIRO_ROOT)/src/miro/VideoS.i

.obj/VideoParameters.o .obj/VideoParameters.so .shobj/VideoParameters.o .shobj/VideoParameters.so: VideoParameters.cpp VideoParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 VideoDevice.h

.obj/VideoImpl.o .obj/VideoImpl.so .shobj/VideoImpl.o .shobj/VideoImpl.so: VideoImpl.cpp VideoImpl.h \
 VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Task.h \
 VideoParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
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
 $(MIRO_ROOT)/src/miro/VideoS.i

.obj/VideoServer.o .obj/VideoServer.so .shobj/VideoServer.o .shobj/VideoServer.so: VideoServer.cpp VideoServer.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/RobotParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 VideoConnection.h \
 VideoDevice.h \
 VideoConsumer.h \
 $(MIRO_ROOT)/src/miro/Task.h \
 VideoImpl.h \
 VideoParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/VideoS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
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
 $(MIRO_ROOT)/src/miro/VideoS.i VideoParameters.h \
 $(MIRO_ROOT)/src/miro/Utils.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
