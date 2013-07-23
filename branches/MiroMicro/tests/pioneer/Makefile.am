## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/pioneer/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PioneerTest


PioneerTest_SOURCES = PioneerTest.cpp

PioneerTest_LDADD = \
	$(top_builddir)/lib/libpioneer.a \
	$(top_builddir)/lib/libpsos.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: PioneerTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
