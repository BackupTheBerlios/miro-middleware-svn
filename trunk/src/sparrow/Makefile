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
## Miro/src/sparrow/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

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
		SparrowConnection \
		SparrowEventHandler \
		SparrowConsumer \
		SparrowParameters \
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


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o .obj/SparrowConnection.o: SparrowConnection.cpp SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowEventHandler.h SparrowConsumer.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o .obj/SparrowEventHandler.o: SparrowEventHandler.cpp SparrowEventHandler.h \
 SparrowConnection.h SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o .obj/SparrowConsumer.o: SparrowConsumer.cpp \
 \
 \
 \
 \
 \
 SparrowConsumer.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowConnection.h \
 \
 \
 \
 \
 \
 \
 SparrowStallImpl.h \
 \
 \
 SparrowButtonsImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowParameters.o .obj/SparrowParameters.o .obj/SparrowParameters.o .obj/SparrowParameters.o: SparrowParameters.cpp SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o .obj/SparrowMotionImpl.o: SparrowMotionImpl.cpp SparrowMotionImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowConsumer.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o .obj/SparrowKickerImpl.o: SparrowKickerImpl.cpp SparrowKickerImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o .obj/SparrowButtonsImpl.o: SparrowButtonsImpl.cpp SparrowButtonsImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowConsumer.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o .obj/SparrowStallImpl.o: SparrowStallImpl.cpp SparrowStallImpl.h \
 \
 \
 \
 \
 SparrowConnection.h \
 SparrowDevice.h \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \


.obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o .obj/SparrowPanTiltImpl.o: SparrowPanTiltImpl.cpp SparrowPanTiltImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 SparrowParameters.h \
 \
 \
 \
 \
 \
 \
 SparrowConnection.h \
 SparrowDevice.h \
 \
 \
 \
 \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
