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
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Synch.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h \
 PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h PanTiltParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h PanTiltData.h

.obj/PanTiltConsumer.o .obj/PanTiltConsumer.so .shobj/PanTiltConsumer.o .shobj/PanTiltConsumer.so: PanTiltConsumer.cpp PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltData.h \
 PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/PanTiltEventHandler.o .obj/PanTiltEventHandler.so .shobj/PanTiltEventHandler.o .shobj/PanTiltEventHandler.so: PanTiltEventHandler.cpp PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/Synch.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltData.h \
 PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/PanTiltData.o .obj/PanTiltData.so .shobj/PanTiltData.o .shobj/PanTiltData.so: PanTiltData.cpp PanTiltData.h

.obj/PanTiltImpl.o .obj/PanTiltImpl.so .shobj/PanTiltImpl.o .shobj/PanTiltImpl.so: PanTiltImpl.cpp PanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanTiltS.h \
 $(MIRO_ROOT)/src/miro/TiltS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i.h \
 $(MIRO_ROOT)/src/miro/TiltC.h \
 $(MIRO_ROOT)/src/miro/TiltC.i.h \
 $(MIRO_ROOT)/src/miro/TiltS.i.h \
 $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i.h \
 $(MIRO_ROOT)/src/miro/PanS.i.h \
 $(MIRO_ROOT)/src/miro/PanTiltC.h \
 $(MIRO_ROOT)/src/miro/PanTiltC.i.h \
 $(MIRO_ROOT)/src/miro/PanTiltS.i.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.i.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.i.h \
 PanTiltData.h PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/miro/Exception.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/Synch.h PanTiltParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h

.obj/PanTiltParameters.o .obj/PanTiltParameters.so .shobj/PanTiltParameters.o .shobj/PanTiltParameters.so: PanTiltParameters.cpp PanTiltParameters.h \
 $(MIRO_ROOT)/src/miro/TtyParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/Singleton.h \
 $(MIRO_ROOT)/src/miro/Synch.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
