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
## Miro/demo/qtSonar/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += -lmiro -lTAO_CosNaming -lTAO

## program to be built
BIN 	= QtRangeSensor

## additional files
FILES	=
QT_SRC  = QtRangeSensor

FILES += $(QT_SRC)

MOC_SRC	= $(addsuffix .moc.cpp, $(QT_SRC))
SRC	= $(addsuffix .cpp,$(FILES)) $(MOC_SRC)
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

.PRECIOUS: $(MOC_SRC)

depend: $(MOC_SRC)

realclean: clean
	-$(RM) $(MOC_SRC))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/QtRangeSensor.o .obj/QtRangeSensor.so .shobj/QtRangeSensor.o .shobj/QtRangeSensor.so: QtRangeSensor.cpp QtRangeSensor.h \
 $(MIRO_ROOT)/src/miro/Client.h $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/RobotParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i

.obj/QtRangeSensor.moc.o .obj/QtRangeSensor.moc.so .shobj/QtRangeSensor.moc.o .shobj/QtRangeSensor.moc.so: QtRangeSensor.moc.cpp QtRangeSensor.h \
 $(MIRO_ROOT)/src/miro/Client.h $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/RobotParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
