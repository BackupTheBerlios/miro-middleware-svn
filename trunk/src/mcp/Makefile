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
## Miro/src/mcp/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = mcp

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES =	Parameters \
	McpDevice \
	McpMessage \
	McpConnection \
	McpConsumer \
	McpEventHandler

DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

default: all

# Only build static library 
static_libs_only = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
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
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/McpDevice.o .obj/McpDevice.o .obj/McpDevice.o .obj/McpDevice.o: McpDevice.cpp McpDevice.h McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/McpMessage.o .obj/McpMessage.o .obj/McpMessage.o .obj/McpMessage.o: McpMessage.cpp McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/McpConnection.o .obj/McpConnection.o .obj/McpConnection.o .obj/McpConnection.o: McpConnection.cpp $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i $(MIRO_ROOT)/src/miro/Exception.h \
 McpConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h McpDevice.h McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h McpEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/McpConsumer.o .obj/McpConsumer.o .obj/McpConsumer.o .obj/McpConsumer.o: McpConsumer.cpp McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h McpConnection.h \
 $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i McpDevice.h McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/McpEventHandler.o .obj/McpEventHandler.o .obj/McpEventHandler.o .obj/McpEventHandler.o: McpEventHandler.cpp McpEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h McpConnection.h \
 $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i McpDevice.h McpMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h McpConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
