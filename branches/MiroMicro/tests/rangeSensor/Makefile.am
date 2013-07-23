## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/rangeSensor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = RangeSensorTest


RangeSensorTest_SOURCES = RangeSensorTest.cpp

RangeSensorTest_LDADD = \
	-lmiro

all-local: RangeSensorTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
