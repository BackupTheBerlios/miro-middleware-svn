## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/sparrowBoard/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SparrowAccel


SparrowAccel_SOURCES = SparrowAccel.cpp

SparrowAccel_LDADD = \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro \
	$(QT_LIBS)

all-local: SparrowAccel
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
