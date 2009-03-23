## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/gripper/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = GripperTest


GripperTest_SOURCES = GripperTest.cpp

GripperTest_LDADD = \
	-lmiro

all-local: GripperTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
