## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/policyController/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PolicyController


sources = \
	PolicyController.h

tomocsources = \
	PolicyController.cpp

PolicyController_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

PolicyController_LDADD = \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: PolicyController
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
