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
## Miro/src/base/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LIBRARIES = libbase.a


sources = \
	Parameters.xml \
	BaseDevice.cpp \
	BaseImpl.cpp \
	BaseConnection.cpp \
	BaseEventHandler.cpp \
	BaseConsumer.cpp \
	BaseStatus.cpp \
	BaseDummyImpl.cpp

libbase_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
base_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
