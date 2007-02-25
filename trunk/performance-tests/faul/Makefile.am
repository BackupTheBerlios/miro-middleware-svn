## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/faul/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


## binaries to be built
bin_PROGRAMS = SerialPolling

## sourcefiles needed for the different binaries to be built
## the prefix of the following macros must have the same name as the respective binary
SerialPolling_SOURCES  = SerialPolling.cpp

## you may add libraries to be linked here
SerialPolling_LDADD  = -lmiro -lmiroSvc

all-local: SerialPolling
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
