## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/videoFilter/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Include macros
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)
MAKEFILE = Makefile

## dynamic libraries

LDLIBS += -lmiro

## static libraries

MIRO_STATIC_LIBS = miroVideo miroSvc

## performing the magic

MIRO_STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(MIRO_STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(MIRO_STATIC_LIBS))


## program to be built
BIN 	= GrayVideoService

## additional files
FILES	=  \
	Parameters \
	FilterGray \
	GrayVideoService

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)


#----------------------------------------------------------------------------
#	Include targets
#----------------------------------------------------------------------------

include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

.PRECIOUS: Parameters.cpp Parameters.h

# $(BIN): $(MIRO_STATIC_DEPS)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Parameters.o .obj/Parameters.so .shobj/Parameters.o .shobj/Parameters.so: Parameters.cpp Parameters.h \
 \
 \
 \


.obj/FilterGray.o .obj/FilterGray.so .shobj/FilterGray.o .shobj/FilterGray.so: FilterGray.cpp FilterGray.h Parameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/hutz/work/Miro.video/src/miro/ExceptionC.i \
 \
 /home/hutz/work/Miro.video/src/miro/TimeC.i \
 \
 \
 /home/hutz/work/Miro.video/src/miro/WorldPointC.i \
 /home/hutz/work/Miro.video/src/miro/PositionC.i \
 /home/hutz/work/Miro.video/src/miro/VideoC.i \
 \
 \


.obj/GrayVideoService.o .obj/GrayVideoService.so .shobj/GrayVideoService.o .shobj/GrayVideoService.so: GrayVideoService.cpp FilterGray.h Parameters.h \
 \
 \
 \
 \
 \
 \
 \
 \
 \
 /home/hutz/work/Miro.video/src/miro/ExceptionC.i \
 \
 /home/hutz/work/Miro.video/src/miro/TimeC.i \
 \
 \
 /home/hutz/work/Miro.video/src/miro/WorldPointC.i \
 /home/hutz/work/Miro.video/src/miro/PositionC.i \
 /home/hutz/work/Miro.video/src/miro/VideoC.i \
 \
 \
 \
 \
 /home/hutz/work/Miro.video/src/miro/ExceptionS.i \
 \
 /home/hutz/work/Miro.video/src/miro/TimeS.i \
 \
 \
 /home/hutz/work/Miro.video/src/miro/WorldPointS.i \
 /home/hutz/work/Miro.video/src/miro/PositionS.i \
 /home/hutz/work/Miro.video/src/miro/VideoS.i \
 \
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
