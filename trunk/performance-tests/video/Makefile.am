## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/performance-tests/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
