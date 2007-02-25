## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/makeParams/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = MakeParams


MakeParams_SOURCES = MakeParams.cpp

MakeParams_LDADD = \
	-lmiroParams

all-local: MakeParams
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
