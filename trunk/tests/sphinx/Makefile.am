## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/sphinx/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SphinxTest


SphinxTest_SOURCES = SphinxTest.cpp

SphinxTest_LDADD = \
	-lmiro

all-local: SphinxTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
