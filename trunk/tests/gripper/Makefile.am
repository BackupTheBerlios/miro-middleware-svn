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
## Miro/tests/gripper/Makefile
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
