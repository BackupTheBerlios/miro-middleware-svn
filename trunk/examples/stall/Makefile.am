## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/stall/Makefile.am
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
