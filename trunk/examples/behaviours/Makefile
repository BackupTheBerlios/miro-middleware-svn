## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/behaviours/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## hack for debug visualization
INCLDIRS += -I$(NIX_ROOT)/src
LDFLAGS +=  -L$(NIX_ROOT)/lib
LDFLAGS +=  -L$(IPP_ROOT)/lib

## dynamic libraries if needed
LDLIBS += -lmiro -lTAO_CosNaming -lTAO

## program to be built
BIN 	= Behaviours

## additional files
FILES	= \
	OnButton \
	OnButtonParameters \
	RangeSensorBehaviour \
	RangeSensorBehaviourParameters \
	RangeSensorAvoid \
	RangeSensorAvoidParameters \
	WallFollow \
	WallFollowParameters \
	TactileStop \
	Straight \
	StraightParameters \
	Wander \
	WanderParameters

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

## you can toggle the debug and optimize flags at this point
# debug = 1
# optimize = 0

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


.obj/OnButton.o .obj/OnButton.so .shobj/OnButton.o .shobj/OnButton.so: OnButton.cpp OnButton.h \
 \
 \
 \
 \
 \
 \
 \
 \
 OnButtonParameters.h \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/ButtonsC.i

.obj/OnButtonParameters.o .obj/OnButtonParameters.so .shobj/OnButtonParameters.o .shobj/OnButtonParameters.so: OnButtonParameters.cpp OnButtonParameters.h \
 \
 \


.obj/RangeSensorBehaviour.o .obj/RangeSensorBehaviour.so .shobj/RangeSensorBehaviour.o .shobj/RangeSensorBehaviour.so: RangeSensorBehaviour.cpp \
 RangeSensorBehaviour.h \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 \
 \
 \
 \
 \
 \
 \
 \
 RangeSensorBehaviourParameters.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/PositionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 \
 \
 \
 \
 \
 \
 \


.obj/RangeSensorBehaviourParameters.o .obj/RangeSensorBehaviourParameters.so .shobj/RangeSensorBehaviourParameters.o .shobj/RangeSensorBehaviourParameters.so: RangeSensorBehaviourParameters.cpp \
 RangeSensorBehaviourParameters.h \
 \
 \
 \


.obj/RangeSensorAvoid.o .obj/RangeSensorAvoid.so .shobj/RangeSensorAvoid.o .shobj/RangeSensorAvoid.so: RangeSensorAvoid.cpp RangeSensorAvoid.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 \
 \
 \
 \
 \
 \
 \
 \
 RangeSensorAvoidParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 \


.obj/RangeSensorAvoidParameters.o .obj/RangeSensorAvoidParameters.so .shobj/RangeSensorAvoidParameters.o .shobj/RangeSensorAvoidParameters.so: RangeSensorAvoidParameters.cpp \
 RangeSensorAvoidParameters.h \
 \
 \
 \


.obj/WallFollow.o .obj/WallFollow.so .shobj/WallFollow.o .shobj/WallFollow.so: WallFollow.cpp WallFollow.h RangeSensorBehaviour.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 \
 /home/gmayer/Miro/src/miro/TimeC.i \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 \
 \
 \
 \
 \
 \
 \
 \
 RangeSensorBehaviourParameters.h \
 \
 \
 WallFollowParameters.h \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 \


.obj/WallFollowParameters.o .obj/WallFollowParameters.so .shobj/WallFollowParameters.o .shobj/WallFollowParameters.so: WallFollowParameters.cpp \
 WallFollowParameters.h RangeSensorBehaviourParameters.h \
 \
 \
 \


.obj/TactileStop.o .obj/TactileStop.so .shobj/TactileStop.o .shobj/TactileStop.so: TactileStop.cpp TactileStop.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionC.i \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/Straight.o .obj/Straight.so .shobj/Straight.o .shobj/Straight.so: Straight.cpp Straight.h \
 \
 \
 \
 \
 \
 \
 \
 \
 StraightParameters.h \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \


.obj/StraightParameters.o .obj/StraightParameters.so .shobj/StraightParameters.o .shobj/StraightParameters.so: StraightParameters.cpp StraightParameters.h \
 \
 \
 \


.obj/Wander.o .obj/Wander.so .shobj/Wander.o .shobj/Wander.so: Wander.cpp Wander.h \
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
 /home/gmayer/Miro/src/miro/VelocityC.i \
 WanderParameters.h

.obj/WanderParameters.o .obj/WanderParameters.so .shobj/WanderParameters.o .shobj/WanderParameters.so: WanderParameters.cpp WanderParameters.h \
 \
 \
 \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
