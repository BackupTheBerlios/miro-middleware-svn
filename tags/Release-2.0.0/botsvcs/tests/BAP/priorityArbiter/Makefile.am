## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
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


noinst_PROGRAMS = \
	PriorityArbitersTest

PriorityArbitersTest_SOURCES = \
	PriorityArbitersTest.cpp


PriorityArbitersTest_LDADD = \
	-L../lib \
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
