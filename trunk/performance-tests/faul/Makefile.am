## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/templates/Makefile.am.bin
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
