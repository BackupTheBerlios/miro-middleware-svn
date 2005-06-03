## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004, 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/tests/BAP/priorityArbiter/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	PriorityArbitersTest

PriorityArbitersTest_SOURCES = \
	PriorityArbitersTest.cpp


PriorityArbitersTest_LDADD = \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES =
CLEANFILES = $(BUILT_SOURCES)

all-local: PriorityArbitersTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
