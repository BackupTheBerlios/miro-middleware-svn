## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/tests/BAP/parsing/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


noinst_PROGRAMS = \
	PolicyParsingTest


PolicyParsingTest_SOURCES = \
	PolicyParsingTest.cpp

PolicyParsingTest_LDADD = \
	-L../lib \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES =
CLEANFILES = $(BUILT_SOURCES)

all-local: PolicyParsingTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
