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
 \


.obj/BaseImpl.o .obj/BaseImpl.o .obj/BaseImpl.o .obj/BaseImpl.o: BaseImpl.cpp \
 \
 \
 \
 BaseEventHandler.h BaseConsumer.h \
 \
 BaseStatus.h \
 BaseDevice.h \
 \
 \
 \
 \
 \
 \
 \
 \
 BaseImpl.h \
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
 BaseConnection.h \
 \
 \
 \
 \
 BaseParameters.h \
 \
 \
 \


.obj/BaseParameters.o .obj/BaseParameters.o .obj/BaseParameters.o .obj/BaseParameters.o: BaseParameters.cpp BaseParameters.h \
 \
 \
 \
 \
 \
 \
 \


.obj/BaseConnection.o .obj/BaseConnection.o .obj/BaseConnection.o .obj/BaseConnection.o: BaseConnection.cpp BaseConnection.h \
 \
 \
 \
 \
 \
 \
 \
 \
 BaseConsumer.h \
 \
 BaseStatus.h \
 BaseDevice.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 BaseEventHandler.h BaseParameters.h \
 \
 \
 \
 \
 \


.obj/BaseEventHandler.o .obj/BaseEventHandler.o .obj/BaseEventHandler.o .obj/BaseEventHandler.o: BaseEventHandler.cpp BaseEventHandler.h \
 BaseConsumer.h \
 \
 BaseStatus.h \
 BaseDevice.h \
 \
 \
 \
 \
 \
 \
 \
 \
 BaseConnection.h \
 \
 \
 \
 \
 \
 \
 \
 BaseParameters.h \
 \
 \
 \
 \
 \


.obj/BaseConsumer.o .obj/BaseConsumer.o .obj/BaseConsumer.o .obj/BaseConsumer.o: BaseConsumer.cpp BaseImpl.h \
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
 \
 \
 \
 \
 \
 \
 BaseConnection.h \
 \
 \
 \
 \
 \
 BaseConsumer.h \
 \
 BaseStatus.h \
 BaseDevice.h

.obj/BaseStatus.o .obj/BaseStatus.o .obj/BaseStatus.o .obj/BaseStatus.o: BaseStatus.cpp BaseStatus.h BaseDevice.h \
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


.obj/BaseDummyImpl.o .obj/BaseDummyImpl.o .obj/BaseDummyImpl.o .obj/BaseDummyImpl.o: BaseDummyImpl.cpp BaseDummyImpl.h \
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
 \
 \
 \
 \
 \
 \
 BaseDevice.h \
 BaseParameters.h \
 \
 \
 \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
