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
## Miro/tests/pioneer/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
