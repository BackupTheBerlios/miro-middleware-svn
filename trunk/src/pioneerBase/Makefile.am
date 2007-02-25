## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/pioneerBase/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PioneerBase


PioneerBase_SOURCES = \
	PioneerBase.cpp \
	PioneerBase.h

PioneerBase_LDADD = \
	$(top_builddir)/lib/libpioneer.a \
	$(top_builddir)/lib/libpsos.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local: PioneerBase
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
