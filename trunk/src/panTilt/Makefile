## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
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
LDLIBS += -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= DpPanTilt

## additional files
FILES = DpPanTilt \
	Parameters \
	PanTiltConnection \
	PanTiltConsumer \
	PanTiltEventHandler \
	PanTiltData \
	PanTiltImpl 

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

$(BIN): $(STATIC_DEPS)

.PRECIOUS: Parameters.cpp Parameters.h

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/DpPanTilt.o .obj/DpPanTilt.so .shobj/DpPanTilt.o .shobj/DpPanTilt.so: DpPanTilt.cpp DpPanTilt.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/ReactorTask.h PanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanTiltS.h \
 $(MIRO_ROOT)/src/miro/TiltS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TiltC.h \
 $(MIRO_ROOT)/src/miro/TiltC.i \
 $(MIRO_ROOT)/src/miro/TiltS.i \
 $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i \
 $(MIRO_ROOT)/src/miro/PanS.i \
 $(MIRO_ROOT)/src/miro/PanTiltC.h \
 $(MIRO_ROOT)/src/miro/PanTiltC.i \
 $(MIRO_ROOT)/src/miro/PanTiltS.i \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.i \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.i \
 PanTiltData.h PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Utils.h

.obj/Parameters.o .obj/Parameters.so .shobj/Parameters.o .shobj/Parameters.so: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/PanTiltConnection.o .obj/PanTiltConnection.so .shobj/PanTiltConnection.o .shobj/PanTiltConnection.so: PanTiltConnection.cpp \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Synch.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h PanTiltMessage.h \
 PanTiltOpcodes.h $(MIRO_ROOT)/src/miro/DevMessage.h \
 PanTiltData.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/PanTiltConsumer.o .obj/PanTiltConsumer.so .shobj/PanTiltConsumer.o .shobj/PanTiltConsumer.so: PanTiltConsumer.cpp PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltData.h PanTiltMessage.h \
 PanTiltOpcodes.h $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/PanTiltEventHandler.o .obj/PanTiltEventHandler.so .shobj/PanTiltEventHandler.o .shobj/PanTiltEventHandler.so: PanTiltEventHandler.cpp PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/Synch.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltData.h PanTiltMessage.h \
 PanTiltOpcodes.h $(MIRO_ROOT)/src/miro/DevMessage.h

.obj/PanTiltData.o .obj/PanTiltData.so .shobj/PanTiltData.o .shobj/PanTiltData.so: PanTiltData.cpp PanTiltData.h

.obj/PanTiltImpl.o .obj/PanTiltImpl.so .shobj/PanTiltImpl.o .shobj/PanTiltImpl.so: PanTiltImpl.cpp PanTiltImpl.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.h \
 $(MIRO_ROOT)/src/miro/PanTiltS.h \
 $(MIRO_ROOT)/src/miro/TiltS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TiltC.h \
 $(MIRO_ROOT)/src/miro/TiltC.i \
 $(MIRO_ROOT)/src/miro/TiltS.i \
 $(MIRO_ROOT)/src/miro/PanS.h \
 $(MIRO_ROOT)/src/miro/PanC.h \
 $(MIRO_ROOT)/src/miro/PanC.i \
 $(MIRO_ROOT)/src/miro/PanS.i \
 $(MIRO_ROOT)/src/miro/PanTiltC.h \
 $(MIRO_ROOT)/src/miro/PanTiltC.i \
 $(MIRO_ROOT)/src/miro/PanTiltS.i \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.h \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltC.i \
 $(MIRO_ROOT)/src/miro/DirectedPerceptionPanTiltS.i \
 PanTiltData.h PanTiltMessage.h PanTiltOpcodes.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/miro/Exception.h PanTiltConnection.h \
 $(MIRO_ROOT)/src/miro/TtyConnection.h PanTiltConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h \
 $(MIRO_ROOT)/src/miro/Log.h PanTiltEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h \
 $(MIRO_ROOT)/src/miro/Synch.h Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
