## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/stall/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = StallPoll


StallPoll_SOURCES = StallPoll.cpp

StallPoll_LDADD = \
	-lmiro

all-local: StallPoll
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
