## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/motionMakefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= MotionTest DifferentialMotionTest SynchroMotionTest

## additional files
FILES	= MotionInterface DifferentialMotionInterface SynchroMotionInterface

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


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/MotionInterface.o .obj/MotionInterface.so .shobj/MotionInterface.o .shobj/MotionInterface.so: MotionInterface.cpp MotionInterface.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionC.i \


.obj/DifferentialMotionInterface.o .obj/DifferentialMotionInterface.so .shobj/DifferentialMotionInterface.o .shobj/DifferentialMotionInterface.so: DifferentialMotionInterface.cpp \
 DifferentialMotionInterface.h MotionInterface.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionC.i \
 \
 /home/gmayer/Miro/src/miro/DifferentialMotionC.i \


.obj/SynchroMotionInterface.o .obj/SynchroMotionInterface.so .shobj/SynchroMotionInterface.o .shobj/SynchroMotionInterface.so: SynchroMotionInterface.cpp \
 SynchroMotionInterface.h MotionInterface.h \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/MotionC.i \
 \
 /home/gmayer/Miro/src/miro/SynchroMotionC.i \


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
