## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/faulMotor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libfaulMotor.a


sources = \
	Parameters.xml \
	FaulMotorConnection.cpp \
	FaulMotorConsumer.cpp \
	FaulMotorMotionImpl.cpp \
	TimerEventHandler.cpp

Parameters.h: Parameters.cpp

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
