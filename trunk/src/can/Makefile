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
## Miro/src/can/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = can

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES =	CanConnection \
	CanMessage \
	CanEventHandler \
	CanParameters

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

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/CanConnection.o .obj/CanConnection.o .obj/CanConnection.o .obj/CanConnection.o: CanConnection.cpp CanConnection.h CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h canmsg.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/DevConnection.h \
 CanParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h canioctl.h

.obj/CanMessage.o .obj/CanMessage.o .obj/CanMessage.o .obj/CanMessage.o: CanMessage.cpp CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h canmsg.h

.obj/CanEventHandler.o .obj/CanEventHandler.o .obj/CanEventHandler.o .obj/CanEventHandler.o: CanEventHandler.cpp CanEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h CanConnection.h CanMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h canmsg.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/DevConnection.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i

.obj/CanParameters.o .obj/CanParameters.o .obj/CanParameters.o .obj/CanParameters.o: CanParameters.cpp CanParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h $(MIRO_ROOT)/src/miro/Parameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
