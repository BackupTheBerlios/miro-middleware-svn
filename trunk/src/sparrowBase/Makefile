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
## Miro/src/sparrowBase/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = sparrow can pioneer psos miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS +=	-lmiro 

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= SparrowBase

## additional files
FILES	= SparrowServer

## performing the magic

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

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/SparrowServer.o .obj/SparrowServer.so .shobj/SparrowServer.o .shobj/SparrowServer.so: SparrowServer.cpp SparrowServer.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ExceptionC.i \
 /home/gmayer/Miro/src/miro/ExceptionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/ScanDescriptionC.i \
 /home/gmayer/Miro/src/miro/ScanDescriptionS.i \
 \
 \
 /home/gmayer/Miro/src/miro/TimeC.i /home/gmayer/Miro/src/miro/TimeS.i \
 \
 /home/gmayer/Miro/src/miro/RangeEventC.i \
 /home/gmayer/Miro/src/miro/RangeEventS.i \
 \
 /home/gmayer/Miro/src/miro/RangeSensorC.i \
 /home/gmayer/Miro/src/miro/RangeSensorS.i \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/WorldPointC.i \
 /home/gmayer/Miro/src/miro/WorldPointS.i \
 \
 /home/gmayer/Miro/src/miro/PositionC.i \
 /home/gmayer/Miro/src/miro/PositionS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/VelocityC.i \
 /home/gmayer/Miro/src/miro/VelocityS.i \
 \
 /home/gmayer/Miro/src/miro/MotionStatusC.i \
 /home/gmayer/Miro/src/miro/MotionStatusS.i \
 \
 /home/gmayer/Miro/src/miro/OdometryC.i \
 /home/gmayer/Miro/src/miro/OdometryS.i \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/MotionC.i \
 /home/gmayer/Miro/src/miro/MotionS.i \
 \
 /home/gmayer/Miro/src/miro/DifferentialMotionC.i \
 /home/gmayer/Miro/src/miro/DifferentialMotionS.i \
 \
 /home/gmayer/Miro/src/miro/SparrowMotionC.i \
 /home/gmayer/Miro/src/miro/SparrowMotionS.i \
 \
 \
 \
 \
 \
 /home/gmayer/Miro/src/miro/KickerC.i \
 /home/gmayer/Miro/src/miro/KickerS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/ButtonsC.i \
 /home/gmayer/Miro/src/miro/ButtonsS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/StallC.i \
 /home/gmayer/Miro/src/miro/StallS.i \
 \
 \
 \
 /home/gmayer/Miro/src/miro/PanC.i /home/gmayer/Miro/src/miro/PanS.i \
 \
 /home/gmayer/Miro/src/miro/SparrowPanTiltC.i \
 /home/gmayer/Miro/src/miro/SparrowPanTiltS.i \
 \
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
