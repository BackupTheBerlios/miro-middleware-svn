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
## Miro/src/faulTty/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = faulTty

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	Parameters \
		FaulTtyDevice \
		FaulTtyMessage \
		FaulTtyConnection \
		FaulTtyEventHandler\
		TimerEventHandler

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
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/FaulTtyDevice.o .obj/FaulTtyDevice.o .obj/FaulTtyDevice.o .obj/FaulTtyDevice.o: FaulTtyDevice.cpp FaulTtyMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/FaulTtyMessage.o .obj/FaulTtyMessage.o .obj/FaulTtyMessage.o .obj/FaulTtyMessage.o: FaulTtyMessage.cpp FaulTtyMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/FaulTtyConnection.o .obj/FaulTtyConnection.o .obj/FaulTtyConnection.o .obj/FaulTtyConnection.o: FaulTtyConnection.cpp \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/Synch.h \
 FaulTtyConnection.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 FaulTtyEventHandler.h $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 FaulTtyMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/FaulTtyEventHandler.o .obj/FaulTtyEventHandler.o .obj/FaulTtyEventHandler.o .obj/FaulTtyEventHandler.o: FaulTtyEventHandler.cpp FaulTtyEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h $(MIRO_ROOT)/src/miro/Synch.h \
 FaulTtyConnection.h $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h FaulTtyMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/TimerEventHandler.o .obj/TimerEventHandler.o .obj/TimerEventHandler.o .obj/TimerEventHandler.o: TimerEventHandler.cpp TimerEventHandler.h \
 FaulTtyConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/TtyConnection.h \
 FaulTtyMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
