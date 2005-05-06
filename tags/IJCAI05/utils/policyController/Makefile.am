## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/policyController/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


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
