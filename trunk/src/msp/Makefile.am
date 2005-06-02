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

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
