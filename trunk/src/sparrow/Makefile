## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/sparrow/Makefile
##
##############################################################################

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
		Parameters \
		AliveCollector \
		AliveEventHandler \
		SparrowBaseConnection \
		SparrowConnection \
		SparrowConnection2003 \
		SparrowEventHandler \
		SparrowEventHandler2003 \
		SparrowConsumer \
		SparrowConsumer2003 \
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

.PRECIOUS: Parameters.h Parameters.cpp

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.o .obj/Parameters.o .obj/Parameters.o: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/AliveCollector.o .obj/AliveCollector.o .obj/AliveCollector.o .obj/AliveCollector.o: AliveCollector.cpp AliveCollector.h

.obj/AliveEventHandler.o .obj/AliveEventHandler.o .obj/AliveEventHandler.o .obj/AliveEventHandler.o: AliveEventHandler.cpp AliveEventHandler.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h SparrowConnection2003.h \
 SparrowDevice2003.h $(MIRO_ROOT)/src/miro/Angle.h \
 Parameters.h $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h AliveCollector.h

.obj/SparrowBaseConnection.o .obj/SparrowBaseConnection.o .obj/SparrowBaseConnection.o .obj/SparrowBaseConnection.o: SparrowBaseConnection.cpp \
 SparrowBaseConnection.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h

.obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o: SparrowConnection.cpp SparrowConnection.h \
 SparrowDevice.h $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h SparrowEventHandler.h \
 SparrowConsumer.h $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h

.obj/SparrowConnection2003.o .obj/SparrowConnection2003.o .obj/SparrowConnection2003.o .obj/SparrowConnection2003.o: SparrowConnection2003.cpp \
 SparrowConnection2003.h SparrowDevice2003.h \
 $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 SparrowEventHandler2003.h SparrowConsumer2003.h AliveCollector.h \
 $(MIRO_ROOT)/src/faulMotor/FaulMotorConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
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
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h SparrowDevice.h

.obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o: SparrowEventHandler.cpp SparrowEventHandler.h \
 SparrowConnection.h SparrowDevice.h \
 $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h

.obj/SparrowEventHandler2003.o .obj/SparrowEventHandler2003.o .obj/SparrowEventHandler2003.o .obj/SparrowEventHandler2003.o: SparrowEventHandler2003.cpp \
 SparrowEventHandler2003.h SparrowConnection2003.h SparrowDevice2003.h \
 $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h

.obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o: SparrowConsumer.cpp \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/ButtonsC.i SparrowConsumer.h \
 SparrowDevice.h $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h SparrowConnection.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h SparrowStallImpl.h \
 $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i SparrowButtonsImpl.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

.obj/SparrowConsumer2003.o .obj/SparrowConsumer2003.o .obj/SparrowConsumer2003.o .obj/SparrowConsumer2003.o: SparrowConsumer2003.cpp \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/ButtonsC.i SparrowConsumer2003.h \
 SparrowDevice2003.h $(MIRO_ROOT)/src/miro/Angle.h \
 AliveCollector.h \
 $(MIRO_ROOT)/src/faulMotor/FaulMotorConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h SparrowConnection2003.h \
 SparrowBaseConnection.h $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h IRLookupTable.h \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/faulTty/FaulTtyMessage.h

.obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o: SparrowMotionImpl.cpp SparrowMotionImpl.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionS.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
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
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
 $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
 $(MIRO_ROOT)/src/miro/SparrowMotionC.h \
 $(MIRO_ROOT)/src/miro/SparrowMotionC.i \
 $(MIRO_ROOT)/src/miro/SparrowMotionS.i \
 $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/Exception.h SparrowConnection.h \
 SparrowDevice.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h SparrowBaseConnection.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h SparrowConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h

.obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o: SparrowKickerImpl.cpp SparrowKickerImpl.h \
 $(MIRO_ROOT)/src/miro/KickerS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/KickerC.h \
 $(MIRO_ROOT)/src/miro/KickerC.i \
 $(MIRO_ROOT)/src/miro/KickerS.i SparrowBaseConnection.h \
 Parameters.h $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 SparrowConnection2003.h SparrowDevice2003.h SparrowConnection.h \
 SparrowDevice.h

.obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o: SparrowButtonsImpl.cpp SparrowButtonsImpl.h \
 SparrowDevice.h $(MIRO_ROOT)/src/miro/Angle.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.i \
 $(MIRO_ROOT)/src/miro/ButtonsS.i SparrowConsumer.h \
 Parameters.h $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/OdometryImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/Thread.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h

.obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o: SparrowStallImpl.cpp SparrowStallImpl.h \
 $(MIRO_ROOT)/src/miro/StallS.h \
 $(MIRO_ROOT)/src/miro/StallC.h \
 $(MIRO_ROOT)/src/miro/StallC.i \
 $(MIRO_ROOT)/src/miro/StallS.i \
 $(MIRO_ROOT)/src/miro/Exception.h SparrowBaseConnection.h \
 Parameters.h $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h SparrowConnection.h \
 SparrowDevice.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 SparrowConnection2003.h SparrowDevice2003.h

.obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o: SparrowPanTiltImpl.cpp SparrowPanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i \
 $(MIRO_ROOT)/src/miro/PanS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltC.h \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltC.i \
 $(MIRO_ROOT)/src/miro/SparrowPanTiltS.i \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Angle.h Parameters.h \
 $(MIRO_ROOT)/src/can/Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 SparrowConnection.h SparrowDevice.h SparrowBaseConnection.h \
 $(MIRO_ROOT)/src/can/CanConnection.h \
 $(MIRO_ROOT)/src/can/CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/can/canmsg.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 SparrowConnection2003.h SparrowDevice2003.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
