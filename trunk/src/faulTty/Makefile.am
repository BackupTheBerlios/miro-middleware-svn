## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/faulTty/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LIBRARIES = libfaulTty.a


sources = \
	FaulControllerConnection.cpp \
	FaulTtyConnection.cpp \
	FaulCanConnection.cpp \
	FaulTtyEventHandler.cpp

libfaulTty_a_SOURCES = $(sources)

#BUILT_SOURCES = Parameters.cpp Parameters.h
#CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
faulTty_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
