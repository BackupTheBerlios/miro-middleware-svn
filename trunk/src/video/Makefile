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
 VideoConsumer.h \
 VideoParameters.h \
 \
 \


.obj/VideoConsumer.o .obj/VideoConsumer.so .shobj/VideoConsumer.o .shobj/VideoConsumer.so: VideoConsumer.cpp VideoConsumer.h \
 \
 VideoConnection.h VideoDevice.h VideoImpl.h \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 /home/gmayer/Miro/src/miro/VideoC.i \
 /home/gmayer/Miro/src/miro/VideoS.i

.obj/VideoParameters.o .obj/VideoParameters.so .shobj/VideoParameters.o .shobj/VideoParameters.so: VideoParameters.cpp VideoParameters.h \
 \
 \


.obj/VideoImpl.o .obj/VideoImpl.so .shobj/VideoImpl.o .shobj/VideoImpl.so: VideoImpl.cpp \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 /home/gmayer/Miro/src/miro/VideoC.i \
 /home/gmayer/Miro/src/miro/VideoS.i

.obj/VideoServer.o .obj/VideoServer.so .shobj/VideoServer.o .shobj/VideoServer.so: VideoServer.cpp VideoServer.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 /home/gmayer/Miro/src/miro/VideoC.i \
 /home/gmayer/Miro/src/miro/VideoS.i VideoParameters.h \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
