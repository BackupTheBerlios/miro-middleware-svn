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
## Miro/demo/qtJoystick/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += -lmiro -lTAO_CosNaming -lTAO

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= QtTouchPad

## additional files
#FILES	= myhelpers
QT_SRC  = MainForm

FILES += $(QT_SRC)

MOC_SRC	= $(addsuffix .moc.cpp, $(QT_SRC))
SRC	= $(addsuffix .cpp,$(FILES)) $(MOC_SRC)
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
DEPS := $(addprefix $(VDIR),$(DEPS))
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(MOC_SRC)

realclean: clean
	-$(RM) $(MOC_SRC))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/MainForm.o .obj/MainForm.so .shobj/MainForm.o .shobj/MainForm.so: MainForm.cpp MainForm.h \
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
