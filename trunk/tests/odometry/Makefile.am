## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/odometry/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = OdometryTest


OdometryTest_SOURCES = OdometryTest.cpp

OdometryTest_LDADD = \
	-lmiro

all-local: OdometryTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
