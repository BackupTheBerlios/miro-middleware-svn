## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/parameters/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = MakeParams


MakeParams_SOURCES = MakeParams.cpp

MakeParams_LDADD = \
	-lACE \
	$(INHERITED_LIBS) \
	$(QT_LIBS) \
	-L$(top_srcdir)/src/params/ -lmiroParams

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
