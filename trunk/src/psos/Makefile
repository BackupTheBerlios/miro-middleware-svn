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
## Miro/src/psos/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = psos

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	PsosDevice \
		PsosMessage \
		PsosConnection \
		PsosParameters \
		PsosEventHandler

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


.obj/PsosDevice.o .obj/PsosDevice.o .obj/PsosDevice.o .obj/PsosDevice.o: PsosDevice.cpp PsosDevice.h PsosMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/PsosMessage.o .obj/PsosMessage.o .obj/PsosMessage.o .obj/PsosMessage.o: PsosMessage.cpp PsosMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h PsosDevice.h

.obj/PsosConnection.o .obj/PsosConnection.o .obj/PsosConnection.o .obj/PsosConnection.o: PsosConnection.cpp \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Synch.h PsosConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 PsosEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 PsosMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
 PsosDevice.h PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h

.obj/PsosParameters.o .obj/PsosParameters.o .obj/PsosParameters.o .obj/PsosParameters.o: PsosParameters.cpp PsosParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/XmlParse.h \
 $(MIRO_ROOT)/src/miro/Angle.h \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/PsosEventHandler.o .obj/PsosEventHandler.o .obj/PsosEventHandler.o .obj/PsosEventHandler.o: PsosEventHandler.cpp PsosEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/Synch.h PsosConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 PsosMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
 PsosDevice.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
