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
## Miro/src/msp/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = B21Buttons

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = \
	ButtonsConnection \
	ButtonsEventHandler \
	ButtonsImpl

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


.obj/ButtonsConnection.o .obj/ButtonsConnection.o .obj/ButtonsConnection.o .obj/ButtonsConnection.o: ButtonsConnection.cpp ButtonsConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i ButtonsEventHandler.h \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/ButtonsC.i \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.i \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h ButtonsDevice.h

.obj/ButtonsEventHandler.o .obj/ButtonsEventHandler.o .obj/ButtonsEventHandler.o .obj/ButtonsEventHandler.o: ButtonsEventHandler.cpp ButtonsEventHandler.h \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/ButtonsC.i \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.i \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h ButtonsDevice.h ButtonsConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

.obj/ButtonsImpl.o .obj/ButtonsImpl.o .obj/ButtonsImpl.o .obj/ButtonsImpl.o: ButtonsImpl.cpp ButtonsImpl.h \
 $(MIRO_ROOT)/src/miro/B21ButtonsS.h \
 $(MIRO_ROOT)/src/miro/ButtonsS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/ButtonsC.h \
 $(MIRO_ROOT)/src/miro/ButtonsC.i \
 $(MIRO_ROOT)/src/miro/ButtonsS.i \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.h \
 $(MIRO_ROOT)/src/miro/B21ButtonsC.i \
 $(MIRO_ROOT)/src/miro/B21ButtonsS.i \
 $(MIRO_ROOT)/src/miro/Exception.h ButtonsDevice.h \
 ButtonsConnection.h ButtonsEventHandler.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
