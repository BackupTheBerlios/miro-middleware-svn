## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/notify/Makefile.am
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
