## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/behaviours/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS +=  -lmiroSvc -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= PriorityArbitersTest PolicyParsingTest BehaviourEngineTest

## additional files
FILES = DummyBehaviour \
	DummyBehaviourB \
	DummyArbiter \
	DummyArbiterMessage

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))
DEPS	= $(addsuffix .d,$(FILES) $(BIN))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

## you can toggle the debug and optimize flags at this point
# debug = 1
optimize = 0

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


.obj/DummyBehaviour.o .obj/DummyBehaviour.so .shobj/DummyBehaviour.o .shobj/DummyBehaviour.so: DummyBehaviour.cpp DummyBehaviour.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviour.h $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h DummyArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/DummyBehaviourB.o .obj/DummyBehaviourB.so .shobj/DummyBehaviourB.o .shobj/DummyBehaviourB.so: DummyBehaviourB.cpp DummyBehaviourB.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviour.h $(MIRO_ROOT)/src/miro/Behaviour.h \
 $(MIRO_ROOT)/src/miro/DynamicWindow.h \
 $(MIRO_ROOT)/src/miro/ActionPattern.h $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/TimedBehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h DummyArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/DummyArbiter.o .obj/DummyArbiter.so .shobj/DummyArbiter.o .shobj/DummyArbiter.so: DummyArbiter.cpp DummyArbiter.h \
 $(MIRO_ROOT)/src/miro/PriorityArbiter.h $(MIRO_ROOT)/src/miro/Arbiter.h \
 $(MIRO_ROOT)/src/miro/Synch.h DummyArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

.obj/DummyArbiterMessage.o .obj/DummyArbiterMessage.so .shobj/DummyArbiterMessage.o .shobj/DummyArbiterMessage.so: DummyArbiterMessage.cpp DummyArbiterMessage.h \
 $(MIRO_ROOT)/src/miro/ArbiterMessage.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
