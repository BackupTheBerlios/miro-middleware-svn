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
## Miro/src/sparrow/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = sparrow

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	\
		SparrowConnection \
		SparrowEventHandler \
		SparrowConsumer \
		SparrowParameters \
		SparrowMotionImpl \
		SparrowKickerImpl \
		SparrowButtonsImpl \
		SparrowStallImpl \
		SparrowPanTiltImpl

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


.obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o: SparrowConnection.cpp SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 \
 SparrowEventHandler.h \
 SparrowConsumer.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i

.obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o: SparrowEventHandler.cpp SparrowEventHandler.h \
 SparrowConnection.h SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o: SparrowConsumer.cpp \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/ButtonsC.i SparrowConsumer.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i SparrowConnection.h \
 \
 \
 \
 \
 \
 SparrowStallImpl.h \
 \
 \
 /home/gmayer/Miro/src/miro/StallC.i \
 /home/gmayer/Miro/src/miro/StallS.i SparrowButtonsImpl.h \
 \
 /home/gmayer/Miro/src/miro/ButtonsS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeEventS.i \
 \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 /home/gmayer/Miro/src/miro/RangeSensorS.i \


.obj/SparrowParameters.o .obj/SparrowParameters.o .obj/SparrowParameters.o .obj/SparrowParameters.o: SparrowParameters.cpp SparrowParameters.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 \
 \


.obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o: SparrowMotionImpl.cpp SparrowMotionImpl.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 \
 /home/gmayer/Miro/src/miro/MotionC.i \
 /home/gmayer/Miro/src/miro/MotionS.i \
 \
 /home/gmayer/Miro/src/miro/DifferentialMotionC.i \
 /home/gmayer/Miro/src/miro/DifferentialMotionS.i \
 \
 /home/gmayer/Miro/src/miro/SparrowMotionC.i \
 /home/gmayer/Miro/src/miro/SparrowMotionS.i \
 \
 \
 \
 \
 \
 SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 SparrowConsumer.h \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i

.obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o: SparrowKickerImpl.cpp SparrowKickerImpl.h \
 \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 /home/gmayer/Miro/src/miro/KickerC.i \
 /home/gmayer/Miro/src/miro/KickerS.i SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o: SparrowButtonsImpl.cpp SparrowButtonsImpl.h \
 SparrowDevice.h \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 /home/gmayer/Miro/src/miro/ButtonsC.i \
 /home/gmayer/Miro/src/miro/ButtonsS.i SparrowConsumer.h \
 SparrowParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i \


.obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o: SparrowStallImpl.cpp SparrowStallImpl.h \
 \
 \
 /home/gmayer/Miro/src/miro/StallC.i \
 /home/gmayer/Miro/src/miro/StallS.i \
 SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o: SparrowPanTiltImpl.cpp SparrowPanTiltImpl.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 /home/gmayer/Miro/src/miro/PanC.i \
 /home/gmayer/Miro/src/miro/PanS.i \
 \
 /home/gmayer/Miro/src/miro/SparrowPanTiltC.i \
 /home/gmayer/Miro/src/miro/SparrowPanTiltS.i \
 \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 SparrowConnection.h \
 SparrowDevice.h \
 \
 \
 \
 \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
