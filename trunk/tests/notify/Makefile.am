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
## Miro/test/notify/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
