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
## Miro/src/pioneer/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = pioneer

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	PioneerConnection \
		PioneerConsumer \
		PioneerStatus \
		PioneerMotionImpl  \
		PioneerStallImpl  \
		PioneerParameters 

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


.obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o: PioneerConnection.cpp PioneerConnection.h \
 \
 \
 \
 PioneerConsumer.h \
 PioneerStatus.h \
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
 PioneerParameters.h \
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


.obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o: PioneerConsumer.cpp PioneerConsumer.h \
 \
 PioneerStatus.h \
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
 PioneerStallImpl.h \
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
 \
 \
 \
 \
 \
 \
 \
 \


.obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o: PioneerStatus.cpp \
 \
 \
 PioneerStatus.h \
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


.obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o: PioneerMotionImpl.cpp PioneerMotionImpl.h \
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
 PioneerConnection.h \
 \
 PioneerConsumer.h \
 PioneerStatus.h \
 \
 \
 \
 \
 \
 \
 \
 \
 PioneerParameters.h \
 \
 \
 \
 \
 \


.obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o: PioneerStallImpl.cpp PioneerStallImpl.h \
 \
 \
 \
 \
 \
 \
 \


.obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o: PioneerParameters.cpp PioneerParameters.h \
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


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
