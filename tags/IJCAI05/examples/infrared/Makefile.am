## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PollInfrared


PollInfrared_SOURCES = PollInfrared.cpp

PollInfrared_LDADD = \
	-lmiro

all-local: PollInfrared
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
