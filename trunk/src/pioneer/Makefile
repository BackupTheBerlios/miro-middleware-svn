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
## Miro/src/pioneer/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = pioneer

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	Parameters \
		PioneerConnection \
		PioneerConsumer \
		PioneerMotionImpl  \
		PioneerStallImpl  \
		CanonPanTiltImpl \
		CanonCameraImpl \
		CanonMessage \
		GripperImpl

DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

# Only build static library 
static_libs_only = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU
#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

.PRECIOUS: Parameters.cpp Parameters.h

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.o .obj/Parameters.o .obj/Parameters.o: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/psos/Parameters.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o: PioneerConnection.cpp PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 PioneerConsumer.h $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 CanonPanTiltImpl.h $(MIRO_ROOT)/src/miro/CanonPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanTiltS.h $(MIRO_ROOT)/src/miro/TiltS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TiltC.h $(MIRO_ROOT)/src/miro/TiltC.i \
 $(MIRO_ROOT)/src/miro/TiltS.i $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/PanC.h $(MIRO_ROOT)/src/miro/PanC.i \
 $(MIRO_ROOT)/src/miro/PanS.i $(MIRO_ROOT)/src/miro/PanTiltC.h \
 $(MIRO_ROOT)/src/miro/PanTiltC.i $(MIRO_ROOT)/src/miro/PanTiltS.i \
 $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
 $(MIRO_ROOT)/src/miro/CanonPanTiltC.i \
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
 PioneerConnection.h Parameters.h \
 $(MIRO_ROOT)/src/psos/Parameters.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/psos/PsosMessage.h $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o: PioneerConsumer.cpp PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 PioneerStallImpl.h $(MIRO_ROOT)/src/miro/PioneerStallS.h \
 $(MIRO_ROOT)/src/miro/StallS.h $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i $(MIRO_ROOT)/src/miro/StallS.i \
 $(MIRO_ROOT)/src/miro/PioneerStallC.h \
 $(MIRO_ROOT)/src/miro/PioneerStallC.i \
 $(MIRO_ROOT)/src/miro/PioneerStallS.i Parameters.h \
 $(MIRO_ROOT)/src/psos/Parameters.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h PioneerMotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/MotionC.i $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h $(MIRO_ROOT)/src/miro/Exception.h \
 CanonPanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.h $(MIRO_ROOT)/src/miro/PanTiltS.h \
 $(MIRO_ROOT)/src/miro/TiltS.h $(MIRO_ROOT)/src/miro/TiltC.h \
 $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
 $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
 $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
 $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
 $(MIRO_ROOT)/src/miro/CanonPanTiltC.i \
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
 PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.h $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/BatteryImpl.h $(MIRO_ROOT)/src/miro/BatteryS.h \
 $(MIRO_ROOT)/src/miro/BatteryC.h $(MIRO_ROOT)/src/miro/BatteryC.i \
 $(MIRO_ROOT)/src/miro/BatteryS.i $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/psos/PsosMessage.h $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o: PioneerMotionImpl.cpp PioneerMotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/MotionC.i $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/Exception.h PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 PioneerConsumer.h $(MIRO_ROOT)/src/miro/DevConsumer.h Parameters.h \
 $(MIRO_ROOT)/src/psos/Parameters.h

.obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o: PioneerStallImpl.cpp PioneerStallImpl.h \
 $(MIRO_ROOT)/src/miro/PioneerStallS.h $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i $(MIRO_ROOT)/src/miro/PioneerStallC.h \
 $(MIRO_ROOT)/src/miro/PioneerStallC.i \
 $(MIRO_ROOT)/src/miro/PioneerStallS.i

.obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o: CanonPanTiltImpl.cpp \
 CanonPanTiltImpl.h \
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
 $(MIRO_ROOT)/src/miro/CanonPanTiltS.i $(MIRO_ROOT)/src/miro/Exception.h \
 PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 Parameters.h $(MIRO_ROOT)/src/psos/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/CanonCameraImpl.o .obj/CanonCameraImpl.o .obj/CanonCameraImpl.o .obj/CanonCameraImpl.o: CanonCameraImpl.cpp \
 CanonCameraImpl.h \
 $(MIRO_ROOT)/src/miro/CanonCameraS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/CanonCameraC.h \
 $(MIRO_ROOT)/src/miro/CanonCameraC.i $(MIRO_ROOT)/src/miro/CanonCameraS.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 Parameters.h $(MIRO_ROOT)/src/psos/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/CanonMessage.o .obj/CanonMessage.o .obj/CanonMessage.o .obj/CanonMessage.o: CanonMessage.cpp CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h $(MIRO_ROOT)/src/miro/Synch.h \
 CanonDevice.h

.obj/GripperImpl.o .obj/GripperImpl.o .obj/GripperImpl.o .obj/GripperImpl.o: GripperImpl.cpp GripperImpl.h \
 $(MIRO_ROOT)/src/miro/GripperS.h $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/GripperC.h \
 $(MIRO_ROOT)/src/miro/GripperC.i $(MIRO_ROOT)/src/miro/GripperS.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 CanonMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h CanonDevice.h \
 PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 Parameters.h $(MIRO_ROOT)/src/psos/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
