## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/tests/BAP/monitor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


## binaries to be built
bin_PROGRAMS = TransitionMonitor

## sourcefiles needed for the different binaries to be built
## the prefix of the following macros must have the same name as the respective binary
TransitionMonitor_SOURCES  = TransitionMonitor.cpp  TransitionMonitor.h

## you may add libraries to be linked here
TransitionMonitor_LDADD  = -lmiro

all-local: TransitionMonitor
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
