## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/psos/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libpsos.a


sources = \
	Parameters.xml \
	PsosDevice.cpp \
	PsosMessage.cpp \
	PsosConnection.cpp \
	PsosEventHandler.cpp

Parameters.h: Parameters.cpp

libpsos_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
psos_include_HEADERS = $(includefiles2)

all-local: libpsos.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
