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
## Miro/src/base/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = base

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES =	BaseDevice \
	BaseImpl \
	BaseParameters \
	BaseConnection \
	BaseEventHandler \
	BaseConsumer \
	BaseStatus \
	BaseDummyImpl

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


.obj/BaseDevice.o .obj/BaseDevice.o .obj/BaseDevice.o .obj/BaseDevice.o: BaseDevice.cpp BaseDevice.h \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/BaseImpl.o .obj/BaseImpl.o .obj/BaseImpl.o .obj/BaseImpl.o: BaseImpl.cpp $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/mcp/McpEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h BaseEventHandler.h \
 BaseConsumer.h $(MIRO_ROOT)/src/mcp/McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h BaseStatus.h BaseDevice.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i BaseImpl.h \
 $(MIRO_ROOT)/src/miro/B21MotionS.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.h \
 $(MIRO_ROOT)/src/miro/MotionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionC.h $(MIRO_ROOT)/src/miro/MotionC.i \
 $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.i \
 $(MIRO_ROOT)/src/miro/OdometryS.h \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/B21MotionC.h \
 $(MIRO_ROOT)/src/miro/B21MotionC.i \
 $(MIRO_ROOT)/src/miro/B21MotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h BaseConnection.h \
 $(MIRO_ROOT)/src/mcp/McpDevice.h \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/mcp/McpConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h BaseParameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/mcp/McpParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h

.obj/BaseParameters.o .obj/BaseParameters.o .obj/BaseParameters.o .obj/BaseParameters.o: BaseParameters.cpp BaseParameters.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/mcp/McpParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Angle.h

.obj/BaseConnection.o .obj/BaseConnection.o .obj/BaseConnection.o .obj/BaseConnection.o: BaseConnection.cpp BaseConnection.h \
 $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/mcp/McpDevice.h \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/mcp/McpConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h BaseConsumer.h \
 $(MIRO_ROOT)/src/mcp/McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h BaseStatus.h BaseDevice.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i BaseEventHandler.h \
 BaseParameters.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/mcp/McpParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h

.obj/BaseEventHandler.o .obj/BaseEventHandler.o .obj/BaseEventHandler.o .obj/BaseEventHandler.o: BaseEventHandler.cpp BaseEventHandler.h \
 BaseConsumer.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/mcp/McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h BaseStatus.h BaseDevice.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i BaseConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/mcp/McpDevice.h \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/mcp/McpConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h BaseParameters.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/mcp/McpParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h

.obj/BaseConsumer.o .obj/BaseConsumer.o .obj/BaseConsumer.o .obj/BaseConsumer.o: BaseConsumer.cpp BaseImpl.h \
 $(MIRO_ROOT)/src/miro/B21MotionS.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.h \
 $(MIRO_ROOT)/src/miro/MotionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionC.h $(MIRO_ROOT)/src/miro/MotionC.i \
 $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.i \
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
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/B21MotionC.h \
 $(MIRO_ROOT)/src/miro/B21MotionC.i \
 $(MIRO_ROOT)/src/miro/B21MotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Synch.h BaseConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/mcp/McpDevice.h \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/mcp/McpConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h BaseConsumer.h \
 $(MIRO_ROOT)/src/mcp/McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h BaseStatus.h BaseDevice.h

.obj/BaseStatus.o .obj/BaseStatus.o .obj/BaseStatus.o .obj/BaseStatus.o: BaseStatus.cpp BaseStatus.h BaseDevice.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/mcp/McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
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
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i $(MIRO_ROOT)/src/miro/Synch.h

.obj/BaseDummyImpl.o .obj/BaseDummyImpl.o .obj/BaseDummyImpl.o .obj/BaseDummyImpl.o: BaseDummyImpl.cpp BaseDummyImpl.h \
 $(MIRO_ROOT)/src/miro/B21MotionS.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.h \
 $(MIRO_ROOT)/src/miro/MotionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/VelocityS.h \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/VelocityS.i \
 $(MIRO_ROOT)/src/miro/MotionC.h $(MIRO_ROOT)/src/miro/MotionC.i \
 $(MIRO_ROOT)/src/miro/MotionS.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.h \
 $(MIRO_ROOT)/src/miro/SynchroMotionC.i \
 $(MIRO_ROOT)/src/miro/SynchroMotionS.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.h \
 $(MIRO_ROOT)/src/miro/ClosedMotionC.i \
 $(MIRO_ROOT)/src/miro/ClosedMotionS.i \
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
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusS.i \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/OdometryS.i \
 $(MIRO_ROOT)/src/miro/B21MotionC.h \
 $(MIRO_ROOT)/src/miro/B21MotionC.i \
 $(MIRO_ROOT)/src/miro/B21MotionS.i \
 $(MIRO_ROOT)/src/miro/MotionImpl.h \
 $(MIRO_ROOT)/src/miro/MotionParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Synch.h BaseDevice.h BaseParameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/mcp/McpParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/IO.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
