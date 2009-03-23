## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/msp/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libmsp.a


sources = \
	Parameters.xml \
	MspConnection.cpp \
	MspDevice.cpp \
	MspEventHandler.cpp \
	MspConsumer.cpp

Parameters.h: Parameters.cpp

libmsp_a_SOURCES = $(sources)


extraheader = \
	MspMessage.h

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
msp_include_HEADERS = $(includefiles2)

all-local: libmsp.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
