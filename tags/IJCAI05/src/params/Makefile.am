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
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


lib_LTLIBRARIES = libmiroParams.la


sources = \
	Type.cpp \
	Parameter.cpp \
	Parser.cpp \
	Generator.cpp \
	TextErrorHandler.cpp

libmiroParams_la_SOURCES = $(sources)
libmiroParams_la_LIBADD = \
	$(ACE_LIBS) \
	$(TAO_LIBS) \
        $(INHERITED_LIBS) \
	$(QT_LIBS)


#BUILT_SOURCES = Parameters.cpp Parameters.h
#CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
params_include_HEADERS = $(includefiles2)

all-local: libmiroParams.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
