## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/params/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

## dynamic libraries if needed
LDLIBS += -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= ParameterExample

## additional files
FILES	= \
	Parameters \
	ParameterExample

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))
DEPS	= $(addsuffix .d,$(FILES) $(BIN))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

## you can toggle the debug and optimize flags at this point
# debug = 1
# optimize = 0

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

Parameters.h: Parameters.xml
	$(MIRO_ROOT)/bin/MakeParams -f Parameters.xml -n Parameters
Parameters.cpp: Parameters.xml
	$(MIRO_ROOT)/bin/MakeParams -f Parameters.xml -n Parameters

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.so .shobj/Parameters.o .shobj/Parameters.so: Parameters.cpp Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/ParameterExample.o .obj/ParameterExample.so .shobj/ParameterExample.o .shobj/ParameterExample.so: ParameterExample.cpp Parameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/Utils.h \
 $(MIRO_ROOT)/src/miro/Exception.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
