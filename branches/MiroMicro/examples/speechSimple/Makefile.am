## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/speechSimple/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SpeechSimple


SpeechSimple_SOURCES = SpeechSimple.cpp

SpeechSimple_LDADD = \
	-lmiro

all-local: SpeechSimple
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
