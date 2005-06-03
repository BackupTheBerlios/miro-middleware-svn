## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2001, 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/test/notify/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = NotifyTest


NotifyTest_SOURCES = \
	NotifyTest.cpp \
	NotifyTest.h

NotifyTest_LDADD = \
	-lmiro

all-local: NotifyTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
