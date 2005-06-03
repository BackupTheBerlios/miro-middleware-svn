## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/LogTest/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = LogTest


LogTest_SOURCES = LogTest.cpp

LogTest_LDADD = \
	-lmiro

all-local: LogTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
