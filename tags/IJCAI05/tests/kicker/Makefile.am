## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/tests/kicker/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = KickerTest


KickerTest_SOURCES = KickerTest.cpp

KickerTest_LDADD = \
	-lmiro

all-local: KickerTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
