## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PanLatency


PanLatency_SOURCES = PanLatency.cpp

PanLatency_LDADD = \
	-lmiro

all-local: PanLatency
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
