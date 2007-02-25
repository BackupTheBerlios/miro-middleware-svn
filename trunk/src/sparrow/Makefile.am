## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/sparrow/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libsparrow.a


sources = \
	Parameters.xml \
	AliveCollector.cpp \
	AliveEventHandler.cpp \
	SparrowBaseConnection.cpp \
	SparrowConnection.cpp \
	SparrowConnection2003.cpp \
	SparrowEventHandler.cpp \
	SparrowEventHandler2003.cpp \
	SparrowConsumer.cpp \
	SparrowConsumer2003.cpp \
	SparrowMotionImpl.cpp \
	SparrowKickerImpl.cpp \
	SparrowButtonsImpl.cpp \
	SparrowStallImpl.cpp \
	SparrowPanTiltImpl.cpp

Parameters.h: Parameters.cpp

libsparrow_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

extraheader = \
	SparrowDevice.h

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
sparrow_include_HEADERS = $(includefiles2) $(extraheader)

EXTRA_DIST = \
	IRLookupTable.h \
	SparrowDevice.h

all-local: libsparrow.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
