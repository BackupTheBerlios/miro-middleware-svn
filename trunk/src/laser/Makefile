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
## Miro/src/laser/Makefile
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
LDLIBS += -lmiro -lTAO_CosNaming -lTAO

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= SickLaserService

## additional files
FILES	= 	SickLaserConnection \
		SickLaserTask \
		SickLaserPollTask \
		SickLaserEvent \
		SickLaserImpl \
		SickLaserParameters


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


.obj/SickLaserConnection.o .obj/SickLaserConnection.so .shobj/SickLaserConnection.o .shobj/SickLaserConnection.so: SickLaserConnection.cpp SickLaserConnection.h \
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
 SickLaserMessage.h \
 SickLaserOpcodes.h \
 SickLaserTask.h \
 SickLaserPollTask.h \
 SickLaserEvent.h \
 SickLaserStatistic.h SickLaserParameters.h \
 \
 \


.obj/SickLaserTask.o .obj/SickLaserTask.so .shobj/SickLaserTask.o .shobj/SickLaserTask.so: SickLaserTask.cpp SickLaserTask.h \
 \
 SickLaserConnection.h \
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
 SickLaserMessage.h \
 SickLaserOpcodes.h \
 SickLaserPollTask.h \
 SickLaserEvent.h SickLaserStatistic.h \


.obj/SickLaserPollTask.o .obj/SickLaserPollTask.so .shobj/SickLaserPollTask.o .shobj/SickLaserPollTask.so: SickLaserPollTask.cpp SickLaserPollTask.h \
 \
 SickLaserConnection.h \
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
 SickLaserMessage.h \
 SickLaserOpcodes.h \
 SickLaserTask.h \
 SickLaserEvent.h SickLaserStatistic.h SickLaserParameters.h \
 \
 \


.obj/SickLaserEvent.o .obj/SickLaserEvent.so .shobj/SickLaserEvent.o .shobj/SickLaserEvent.so: SickLaserEvent.cpp SickLaserEvent.h SickLaserTask.h \
 \
 SickLaserStatistic.h \
 SickLaserOpcodes.h SickLaserMessage.h \


.obj/SickLaserImpl.o .obj/SickLaserImpl.so .shobj/SickLaserImpl.o .shobj/SickLaserImpl.so: SickLaserImpl.cpp SickLaserImpl.h \
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


.obj/SickLaserParameters.o .obj/SickLaserParameters.so .shobj/SickLaserParameters.o .shobj/SickLaserParameters.so: SickLaserParameters.cpp SickLaserParameters.h \
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
