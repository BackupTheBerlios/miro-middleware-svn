## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/base/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


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

Parameters.h: Parameters.cpp

libbase_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
base_include_HEADERS = $(includefiles2)

all-local: libbase.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
