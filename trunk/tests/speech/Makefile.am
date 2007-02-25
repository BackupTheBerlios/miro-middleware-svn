## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/speech/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SpeechTest


SpeechTest_SOURCES = SpeechTest.cpp

SpeechTest_LDADD = \
	-lmiro

all-local: SpeechTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
