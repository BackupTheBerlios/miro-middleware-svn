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
## Miro/templates/Makefile.bin
##
##############################################################################

# a makefile example
# for a directory, wich has one binary to be built

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += -lmiro -lTAO_CosNaming -lTAO

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= Behaviours

## additional files
FILES	= \
	OnButton \
	OnButtonParameters \
	RangeSensorAvoid \
	RangeSensorAvoidParameters \
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
optimize = 0

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
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h \
 OnButtonParameters.h \
 $(MIRO_ROOT)/src/miro/EventBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.i.h \
 $(MIRO_ROOT)/src/miro/IO.h

.obj/OnButtonParameters.o .obj/OnButtonParameters.so .shobj/OnButtonParameters.o .shobj/OnButtonParameters.so: OnButtonParameters.cpp OnButtonParameters.h \
 $(MIRO_ROOT)/src/miro/EventBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/KeyValueList.h

.obj/RangeSensorAvoid.o .obj/RangeSensorAvoid.so .shobj/RangeSensorAvoid.o .shobj/RangeSensorAvoid.so: RangeSensorAvoid.cpp RangeSensorAvoid.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i.h \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h \
 RangeSensorAvoidParameters.h \
 $(MIRO_ROOT)/src/miro/EventBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/RobotParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/MotionC.i.h \
 $(MIRO_ROOT)/src/miro/Angle.h

.obj/RangeSensorAvoidParameters.o .obj/RangeSensorAvoidParameters.so .shobj/RangeSensorAvoidParameters.o .shobj/RangeSensorAvoidParameters.so: RangeSensorAvoidParameters.cpp \
 RangeSensorAvoidParameters.h \
 $(MIRO_ROOT)/src/miro/EventBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/KeyValueList.h \
 $(MIRO_ROOT)/src/miro/Angle.h

.obj/TactileStop.o .obj/TactileStop.so .shobj/TactileStop.o .shobj/TactileStop.so: TactileStop.cpp TactileStop.h \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i.h \
 $(MIRO_ROOT)/src/miro/MotionC.i.h \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/Straight.o .obj/Straight.so .shobj/Straight.o .shobj/Straight.so: Straight.cpp Straight.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 StraightParameters.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/MotionC.i.h

.obj/StraightParameters.o .obj/StraightParameters.so .shobj/StraightParameters.o .shobj/StraightParameters.so: StraightParameters.cpp StraightParameters.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/KeyValueList.h \
 $(MIRO_ROOT)/src/miro/Angle.h

.obj/Wander.o .obj/Wander.so .shobj/Wander.o .shobj/Wander.so: Wander.cpp Wander.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/MotionC.i.h WanderParameters.h

.obj/WanderParameters.o .obj/WanderParameters.so .shobj/WanderParameters.o .shobj/WanderParameters.so: WanderParameters.cpp WanderParameters.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/KeyValueList.h \
 $(MIRO_ROOT)/src/miro/Angle.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
