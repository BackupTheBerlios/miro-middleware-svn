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
## Miro/src/can/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LIBRARIES = libcan.a


sources = \
	Parameters.xml \
	CanConnection.cpp \
	CanMessage.cpp \
	CanEventHandler.cpp

libcan_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
can_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
