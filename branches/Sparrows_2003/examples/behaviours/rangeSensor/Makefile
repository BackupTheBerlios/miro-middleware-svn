## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.lib
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = miroBehavioursRangeSensor

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = Parameters \
	RangeSensorBehaviour \
	RangeSensorAvoid \
	RangeSensorWallFollow \
	BehavioursFactory
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

## you can toggle the debug and optimize flags at this point
# debug = 1
# optimize = 0
# Only build static library 
# static_libs_only = 1

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

depend: Parameters.h

Parameters.h: Parameters.xml
	$(MIRO_ROOT)/bin/MakeParams -f Parameters.xml -n Parameters
Parameters.cpp: Parameters.xml
	$(MIRO_ROOT)/bin/MakeParams -f Parameters.xml -n Parameters

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.so .shobj/Parameters.o .shobj/Parameters.so: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/RangeSensorBehaviour.o .obj/RangeSensorBehaviour.so .shobj/RangeSensorBehaviour.o .shobj/RangeSensorBehaviour.so: RangeSensorBehaviour.cpp \
 RangeSensorBehaviour.h $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Policy.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h

.obj/RangeSensorAvoid.o .obj/RangeSensorAvoid.so .shobj/RangeSensorAvoid.o .shobj/RangeSensorAvoid.so: RangeSensorAvoid.cpp RangeSensorAvoid.h \
 RangeSensorBehaviour.h $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Policy.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/RangeSensorWallFollow.o .obj/RangeSensorWallFollow.so .shobj/RangeSensorWallFollow.o .shobj/RangeSensorWallFollow.so: RangeSensorWallFollow.cpp \
 RangeSensorWallFollow.h RangeSensorBehaviour.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Policy.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/BehavioursFactory.o .obj/BehavioursFactory.so .shobj/BehavioursFactory.o .shobj/BehavioursFactory.so: BehavioursFactory.cpp BehavioursFactory.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/MotionC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionC.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/MotionArbiter.h \
 $(MIRO_ROOT)/src/miro/PriorityArbiter.h \
 $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/MotionArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h RangeSensorAvoid.h \
 RangeSensorBehaviour.h \
 $(MIRO_ROOT)/src/miro/EventBehaviour.h \
 $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h \
 $(MIRO_ROOT)/src/miro/Policy.h \
 $(MIRO_ROOT)/src/miro/StructuredPushConsumer.h Parameters.h \
 RangeSensorWallFollow.h $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/BehaviourRepository.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
