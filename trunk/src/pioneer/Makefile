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
FILES = 	PioneerConnection \
		PioneerConsumer \
		PioneerStatus \
		PioneerMotionImpl  \
		PioneerStallImpl  \
		PioneerParameters \
		CanonPanTiltImpl \
		CanonPanTiltMessage

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


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o: PioneerConnection.cpp PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h PioneerStatus.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Angle.h PioneerParameters.h \
 $(MIRO_ROOT)/src/psos/PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/psos/PsosMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o: PioneerConsumer.cpp PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h PioneerStatus.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Angle.h PioneerStallImpl.h \
 $(MIRO_ROOT)/src/miro/PioneerStallS.h \
 $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i \
 $(MIRO_ROOT)/src/miro/PioneerStallC.h \
 $(MIRO_ROOT)/src/miro/PioneerStallC.i \
 $(MIRO_ROOT)/src/miro/PioneerStallS.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/psos/PsosMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o: PioneerStatus.cpp \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i PioneerStatus.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Angle.h \
 $(MIRO_ROOT)/src/psos/PsosMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o: PioneerMotionImpl.cpp PioneerMotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/MotionC.i \
 $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h PioneerConnection.h \
 $(MIRO_ROOT)/src/psos/PsosConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PioneerConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h PioneerStatus.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/Angle.h PioneerParameters.h \
 $(MIRO_ROOT)/src/psos/PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/Singleton.h

.obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o: PioneerStallImpl.cpp PioneerStallImpl.h \
 $(MIRO_ROOT)/src/miro/PioneerStallS.h \
 $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i \
 $(MIRO_ROOT)/src/miro/PioneerStallC.h \
 $(MIRO_ROOT)/src/miro/PioneerStallC.i \
 $(MIRO_ROOT)/src/miro/PioneerStallS.i

.obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o: PioneerParameters.cpp PioneerParameters.h \
 $(MIRO_ROOT)/src/psos/PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionHelper.h \
 $(MIRO_ROOT)/src/miro/Angle.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
