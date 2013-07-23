## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/fusedRangeSensor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = FusedRangeSensorTest


FusedRangeSensorTest_SOURCES = FusedRangeSensorTest.cpp

FusedRangeSensorTest_LDADD = \
	-lmiro

all-local: FusedRangeSensorTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
