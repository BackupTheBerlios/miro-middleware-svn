## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/params/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LTLIBRARIES = libmiroParams.la


sources = \
	Class.cpp \
	Parameter.cpp \
	Parser.cpp \
	Generator.cpp \
	TextErrorHandler.cpp

libmiroParams_la_SOURCES = $(sources)

#BUILT_SOURCES = Parameters.cpp Parameters.h
#CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
params_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
