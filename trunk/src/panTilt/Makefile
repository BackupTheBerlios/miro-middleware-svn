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
## Miro/src/panTilt/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))


## dynamic libraries if needed
LDLIBS +=	-lmiro \
		-lTAO_CosNotification \
		-lTAO_CosNaming  \
		-lTAO

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= DpPanTilt

## additional files
FILES	= \
	PanTiltConnection \
	PanTiltConsumer \
	PanTiltEventHandler \
	PanTiltData \
	PanTiltImpl \
	PanTiltParameters

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/PanTiltConnection.o .obj/PanTiltConnection.so .shobj/PanTiltConnection.o .shobj/PanTiltConnection.so: PanTiltConnection.cpp \
 \
 \
 \
 PanTiltConnection.h \
 \
 PanTiltEventHandler.h \
 PanTiltMessage.h \
 PanTiltOpcodes.h \
 PanTiltParameters.h \
 \
 \
 \
 PanTiltData.h

.obj/PanTiltConsumer.o .obj/PanTiltConsumer.so .shobj/PanTiltConsumer.o .shobj/PanTiltConsumer.so: PanTiltConsumer.cpp PanTiltConsumer.h \
 \
 PanTiltData.h \
 PanTiltMessage.h PanTiltOpcodes.h \
 \


.obj/PanTiltEventHandler.o .obj/PanTiltEventHandler.so .shobj/PanTiltEventHandler.o .shobj/PanTiltEventHandler.so: PanTiltEventHandler.cpp PanTiltEventHandler.h \
 \
 PanTiltConnection.h \
 \
 PanTiltConsumer.h \
 \
 PanTiltData.h \
 PanTiltMessage.h PanTiltOpcodes.h \


.obj/PanTiltData.o .obj/PanTiltData.so .shobj/PanTiltData.o .shobj/PanTiltData.so: PanTiltData.cpp PanTiltData.h

.obj/PanTiltImpl.o .obj/PanTiltImpl.so .shobj/PanTiltImpl.o .shobj/PanTiltImpl.so: PanTiltImpl.cpp PanTiltImpl.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 PanTiltData.h PanTiltMessage.h PanTiltOpcodes.h \
 \
 PanTiltConnection.h \
 PanTiltConsumer.h \
 \
 PanTiltEventHandler.h \
 \
 PanTiltParameters.h \
 \
 \
 \


.obj/PanTiltParameters.o .obj/PanTiltParameters.so .shobj/PanTiltParameters.o .shobj/PanTiltParameters.so: PanTiltParameters.cpp PanTiltParameters.h \
 \
 \
 \
 \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
