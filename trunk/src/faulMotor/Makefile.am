## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/faulMotor/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LIBRARIES = libfaulMotor.a


sources = \
	Parameters.xml \
	FaulMotorConnection.cpp \
	FaulMotorConsumer.cpp \
	FaulMotorMotionImpl.cpp \
	TimerEventHandler.cpp

libfaulMotor_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
faulMotor_include_HEADERS = $(includefiles2)

all-local: libfaulMotor.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
