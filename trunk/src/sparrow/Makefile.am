## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/sparrow/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


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

libsparrow_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
sparrow_include_HEADERS = $(includefiles2)

EXTRA_DIST = \
	IRLookupTable.h \
	SparrowDevice.h

all-local:
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
