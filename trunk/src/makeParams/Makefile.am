## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/parameters/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = MakeParams


MakeParams_SOURCES = MakeParams.cpp

MakeParams_LDADD = \
	-lmiroParams

all-local: MakeParams
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
