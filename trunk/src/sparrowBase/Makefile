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
## Miro/src/sparrowBase/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one binary to be built

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

## static libraries

STATIC_LIBS = sparrow can pioneer psos miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS +=	-lmiro \
		-lTAO_CosNaming \
		-lTAO \
                -lTAO_RTEvent
# Additional libs needed for NotifyMulticast:
#   TAO_RTEvent

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= SparrowBase

## additional files
FILES	= SparrowServer

## performing the magic

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


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/SparrowServer.o .obj/SparrowServer.so .shobj/SparrowServer.o .shobj/SparrowServer.so: SparrowServer.cpp SparrowServer.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/RobotParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ReactorTask.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i.h \
 $(MIRO_ROOT)/src/miro/RangeEventS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i.h \
 $(MIRO_ROOT)/src/miro/TimeS.i.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i.h \
 $(MIRO_ROOT)/src/miro/RangeEventS.i.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i.h \
 $(MIRO_ROOT)/src/miro/NotifyMulticast.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i.h \
 $(MIRO_ROOT)/src/miro/PositionS.i.h \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i.h \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i.h \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i.h \
 $(MIRO_ROOT)/src/miro/OdometryS.i.h \
 $(MIRO_ROOT)/src/can/CanEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/sparrow/SparrowConnection.h \
 $(MIRO_ROOT)/src/sparrow/SparrowDevice.h \
 $(MIRO_ROOT)/src/miro/Angle.h \
 $(MIRO_ROOT)/src/sparrow/SparrowParameters.h \
 $(MIRO_ROOT)/src/can/CanParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 $(MIRO_ROOT)/src/sparrow/SparrowConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/sparrow/SparrowMotionImpl.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionS.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
 $(MIRO_ROOT)/src/miro/MotionS.h \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/MotionC.i.h \
 $(MIRO_ROOT)/src/miro/MotionS.i.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.i.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.i.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionC.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionC.i.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionS.i.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/sparrow/SparrowKickerImpl.h \
 $(MIRO_ROOT)/src/miro/KickerS.h \
 $(MIRO_ROOT)/src/miro/KickerC.h \
 $(MIRO_ROOT)/src/miro/KickerC.i.h \
 $(MIRO_ROOT)/src/miro/KickerS.i.h \
 $(MIRO_ROOT)/src/sparrow/SparrowButtonsImpl.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.i.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.i.h \
 $(MIRO_ROOT)/src/sparrow/SparrowStallImpl.h \
 $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i.h \
 $(MIRO_ROOT)/src/miro/StallS.i.h \
 $(MIRO_ROOT)/src/sparrow/SparrowPanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i.h \
 $(MIRO_ROOT)/src/miro/PanS.i.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltC.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltC.i.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltS.i.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/psos/PsosEventHandler.h \
 $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 $(MIRO_ROOT)/src/pioneer/PioneerConsumer.h \
 $(MIRO_ROOT)/src/pioneer/PioneerStatus.h \
 $(MIRO_ROOT)/src/miro/Utils.h \
 $(MIRO_ROOT)/src/pioneer/PioneerParameters.h \
 $(MIRO_ROOT)/src/psos/PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
