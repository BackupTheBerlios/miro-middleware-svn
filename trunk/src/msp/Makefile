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
## Miro/src/msp/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = msp

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = \
	MspConnection \
	MspParameters \
	MspDevice \
	MspEventHandler \
	MspConsumer

REST =	sonarImpl \
	infraredImpl \
	tactileImpl

#	sonarImpl
#	mspStruturedPushSupplier


DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

# Only build static library 
static_libs_only = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU
#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/MspConnection.o .obj/MspConnection.o .obj/MspConnection.o .obj/MspConnection.o: MspConnection.cpp \
 $(MIRO_ROOT)/src/abus/AbusDevice.h MspConnection.h \
 $(MIRO_ROOT)/src/abus/AbusConnection.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h MspDevice.h MspMessage.h \
 $(MIRO_ROOT)/src/abus/AbusMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h MspParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i

.obj/MspParameters.o .obj/MspParameters.o .obj/MspParameters.o .obj/MspParameters.o: MspParameters.cpp MspParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionHelper.h \
 $(MIRO_ROOT)/src/miro/Angle.h

.obj/MspDevice.o .obj/MspDevice.o .obj/MspDevice.o .obj/MspDevice.o: MspDevice.cpp MspDevice.h

.obj/MspEventHandler.o .obj/MspEventHandler.o .obj/MspEventHandler.o .obj/MspEventHandler.o: MspEventHandler.cpp MspEventHandler.h \
 $(MIRO_ROOT)/src/abus/AbusEventHandler.h \
 $(MIRO_ROOT)/src/miro/DevEventHandler.h MspConnection.h \
 $(MIRO_ROOT)/src/abus/AbusConnection.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h MspMessage.h MspDevice.h \
 $(MIRO_ROOT)/src/abus/AbusMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/abus/AbusDevice.h

.obj/MspConsumer.o .obj/MspConsumer.o .obj/MspConsumer.o .obj/MspConsumer.o: MspConsumer.cpp $(MIRO_ROOT)/src/miro/RangeEventC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/RangeEventC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
 $(MIRO_ROOT)/src/miro/RangeSensorS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
 $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
 $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.h \
 $(MIRO_ROOT)/src/miro/TimeS.h $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/RangeEventS.i \
 $(MIRO_ROOT)/src/miro/RangeSensorC.h \
 $(MIRO_ROOT)/src/miro/RangeSensorC.i \
 $(MIRO_ROOT)/src/miro/RangeSensorS.i \
 $(MIRO_ROOT)/src/miro/Synch.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h MspConsumer.h \
 $(MIRO_ROOT)/src/miro/DevConsumer.h MspDevice.h MspMessage.h \
 $(MIRO_ROOT)/src/abus/AbusMessage.h \
 $(MIRO_ROOT)/src/miro/DevMessage.h \
 $(MIRO_ROOT)/src/abus/AbusDevice.h MspConnection.h \
 $(MIRO_ROOT)/src/abus/AbusConnection.h \
 $(MIRO_ROOT)/src/miro/DevConnection.h \
 $(MIRO_ROOT)/src/miro/Exception.h MspParameters.h \
 $(MIRO_ROOT)/src/miro/DevParameters.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/base/BaseStatus.h \
 $(MIRO_ROOT)/src/base/BaseDevice.h \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
