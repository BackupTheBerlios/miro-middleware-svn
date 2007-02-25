## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/config/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


## binaries to be built
bin_PROGRAMS = ConfigTest

## sourcefiles needed for the different binaries to be built
## the prefix of the following macros must have the same name as the respective binary
ConfigTest_SOURCES  = ConfigTest.cpp

## you may add libraries to be linked here (normally, libmiro should be enough, it is
## allready pre-linked against all necessary ACE, TAO and Qt variables)
ConfigTest_LDADD  = -lmiro

all-local: ConfigTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
