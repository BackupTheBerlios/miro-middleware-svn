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
## Miro/tests/sparrow/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = CanPerformance


CanPerformance_SOURCES = CanPerformance.cpp

CanPerformance_LDADD = \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: CanPerformance
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
