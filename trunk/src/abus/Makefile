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
## Miro/src/abus/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = abus

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

# Only build static library 
static_libs_only = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU

## specify your library files here
FILES = AbusConnection \
	AbusDevice \
	AbusEventHandler

DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

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


.obj/AbusConnection.o .obj/AbusConnection.o .obj/AbusConnection.o .obj/AbusConnection.o: AbusConnection.cpp \
 AbusConnection.h \
 AbusDevice.h \
 AbusMessage.h

.obj/AbusDevice.o .obj/AbusDevice.o .obj/AbusDevice.o .obj/AbusDevice.o: AbusDevice.cpp AbusDevice.h

.obj/AbusEventHandler.o .obj/AbusEventHandler.o .obj/AbusEventHandler.o .obj/AbusEventHandler.o: AbusEventHandler.cpp AbusEventHandler.h \
 AbusConnection.h \
 \
 AbusMessage.h \
 AbusDevice.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
