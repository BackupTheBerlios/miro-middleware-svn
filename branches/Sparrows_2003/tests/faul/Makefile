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
## Miro/tests/faulMotor/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries
##STATIC_LIBS = faulMotor faulTty miroSvc
STATIC_LIBS = can sparrow faulTty faulMotor  miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))


## dynamic libraries if needed
LDLIBS += -lmiro 

## program to be built
BIN 	= FaulTest

## additional files
FILES	= FaulTest

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


.obj/FaulTest.o .obj/FaulTest.so .shobj/FaulTest.o .shobj/FaulTest.so: FaulTest.cpp \
 $(MIRO_ROOT)/src/faulMotor/FaulMotorConnection.h \
 $(MIRO_ROOT)/src/faulTty/FaulTtyConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 ##$(MIRO_ROOT)/src/faulMotor/CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h $(MIRO_ROOT)/src/faulMotor/CanonDevice.h \
 $(MIRO_ROOT)/src/faulMotor/Parameters.h $(MIRO_ROOT)/src/faulTty/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/faulMotor/FaulMotorConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 ##$(MIRO_ROOT)/src/faulMotor/PioneerStallImpl.h \
 $(MIRO_ROOT)/src/miro/PioneerStallS.h $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i $(MIRO_ROOT)/src/miro/PioneerStallC.h \
 $(MIRO_ROOT)/src/miro/PioneerStallC.i \
 $(MIRO_ROOT)/src/miro/PioneerStallS.i \
 $(MIRO_ROOT)/src/faulMotor/CanonPanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.h $(MIRO_ROOT)/src/miro/PanTiltS.h \
 $(MIRO_ROOT)/src/miro/TiltS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/TiltC.h \
 $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
 $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
 $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
 $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
 $(MIRO_ROOT)/src/miro/CanonPanTiltC.i \
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
 $(MIRO_ROOT)/src/faulMotor/CanonCameraImpl.h \
 ##$(MIRO_ROOT)/src/miro/CanonCameraS.h $(MIRO_ROOT)/src/miro/CanonCameraC.h \
 $(MIRO_ROOT)/src/miro/CanonCameraC.i $(MIRO_ROOT)/src/miro/CanonCameraS.i \
 ##$(MIRO_ROOT)/src/faulMotor/GripperImpl.h $(MIRO_ROOT)/src/miro/GripperS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/GripperC.h $(MIRO_ROOT)/src/miro/GripperC.i \
 $(MIRO_ROOT)/src/miro/GripperS.i $(MIRO_ROOT)/src/faulTty/PsosEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/ReactorTask.h $(MIRO_ROOT)/src/miro/IO.h \
 $(MIRO_ROOT)/src/miro/Utils.h $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.h $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/BatteryImpl.h $(MIRO_ROOT)/src/miro/BatteryS.h \
 $(MIRO_ROOT)/src/miro/BatteryC.h $(MIRO_ROOT)/src/miro/BatteryC.i \
 $(MIRO_ROOT)/src/miro/BatteryS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
