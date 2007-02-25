## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/video/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = GrabImages


GrabImages_SOURCES = GrabImages.cpp

GrabImages_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: GrabImages
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
