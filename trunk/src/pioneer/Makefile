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
		PioneerParameters 

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
 \
 \
 \
 PioneerConsumer.h \
 PioneerStatus.h \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/PositionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 PioneerParameters.h \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 \
 \


.obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o: PioneerConsumer.cpp PioneerConsumer.h \
 \
 PioneerStatus.h \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/PositionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 PioneerStallImpl.h \
 \
 \
 \
 /home/gmayer/Miro/src/miro/StallC.i \
 /home/gmayer/Miro/src/miro/StallS.i \
 \
 /home/gmayer/Miro/src/miro/PioneerStallC.i \
 /home/gmayer/Miro/src/miro/PioneerStallS.i \
 \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 /home/gmayer/Miro/src/miro/ScanDescriptionS.i \
 \
 /home/gmayer/Miro/src/miro/TimeS.i \
 /home/gmayer/Miro/src/miro/RangeEventS.i \
 \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 /home/gmayer/Miro/src/miro/RangeSensorS.i \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i \
 \
 \
 \


.obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o: PioneerStatus.cpp \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 PioneerStatus.h \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/PositionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 \
 \


.obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o: PioneerMotionImpl.cpp PioneerMotionImpl.h \
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
 \
 \
 /home/gmayer/Miro/src/miro/DifferentialMotionC.i \
 /home/gmayer/Miro/src/miro/DifferentialMotionS.i \
 \
 \
 \
 \
 PioneerConnection.h \
 \
 PioneerConsumer.h \
 PioneerStatus.h \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 PioneerParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \


.obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o: PioneerStallImpl.cpp PioneerStallImpl.h \
 \
 \
 \
 /home/gmayer/Miro/src/miro/StallC.i \
 /home/gmayer/Miro/src/miro/StallS.i \
 \
 /home/gmayer/Miro/src/miro/PioneerStallC.i \
 /home/gmayer/Miro/src/miro/PioneerStallS.i

.obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o: PioneerParameters.cpp PioneerParameters.h \
 \
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


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
