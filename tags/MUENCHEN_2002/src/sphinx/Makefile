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
## Miro/src/sphinx/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = 

LDFLAGS += -L$(FESTIVAL_ROOT)/src/lib -L$(SPEECH_TOOLS_ROOT)/lib
LIBS += -lmiroSvc -lsphinx2 -lsphinx2ad -lsphinx2fe -lFestival -lestools -leststring -lestbase -ltermcap 

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS += 	-lmiro 

## include directories
INCLDIRS += -I$(FESTIVAL_ROOT)/src/include -I$(SPEECH_TOOLS_ROOT)/include

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= SphinxSpeech

## additional files
FILES	=	FestivalSpeech \
		SphinxSpeechImpl \
		SphinxSpeechTask

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


.obj/FestivalSpeech.o .obj/FestivalSpeech.so .shobj/FestivalSpeech.o .shobj/FestivalSpeech.so: FestivalSpeech.cpp FestivalSpeech.h

.obj/SphinxSpeechImpl.o .obj/SphinxSpeechImpl.so .shobj/SphinxSpeechImpl.o .shobj/SphinxSpeechImpl.so: SphinxSpeechImpl.cpp SphinxSpeechImpl.h \
 $(MIRO_ROOT)/src/miro/SpeechS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/SpeechC.h \
 $(MIRO_ROOT)/src/miro/SpeechC.i \
 $(MIRO_ROOT)/src/miro/SpeechS.i SphinxSpeechTask.h \
 $(MIRO_ROOT)/src/miro/Task.h $(MIRO_ROOT)/src/miro/Log.h \
 FestivalSpeech.h

.obj/SphinxSpeechTask.o .obj/SphinxSpeechTask.so .shobj/SphinxSpeechTask.o .shobj/SphinxSpeechTask.so: SphinxSpeechTask.cpp SphinxSpeechTask.h \
 $(MIRO_ROOT)/src/miro/Task.h $(MIRO_ROOT)/src/miro/Log.h \
 SphinxSpeechImpl.h $(MIRO_ROOT)/src/miro/SpeechS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/SpeechC.h \
 $(MIRO_ROOT)/src/miro/SpeechC.i \
 $(MIRO_ROOT)/src/miro/SpeechS.i \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
