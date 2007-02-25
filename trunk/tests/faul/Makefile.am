## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/faul/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = FaulTest


FaulTest_SOURCES = FaulTest.cpp

FaulTest_LDADD = \
	$(top_builddir)/lib/libfaulMotor.a \
	$(top_builddir)/lib/libfaulTty.a \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: FaulTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
