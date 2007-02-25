## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/infrared/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PollInfrared


PollInfrared_SOURCES = PollInfrared.cpp

PollInfrared_LDADD = \
	-lmiro

all-local: PollInfrared
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
