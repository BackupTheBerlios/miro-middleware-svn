## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/dpPanTilt/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = DpPanTiltTest


DpPanTiltTest_SOURCES = DpPanTiltTest.cpp

DpPanTiltTest_LDADD = \
	-lmiro

all-local: DpPanTiltTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
