## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/panTilt/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PanLatency


PanLatency_SOURCES = PanLatency.cpp

PanLatency_LDADD = \
	-lmiro

all-local: PanLatency
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
