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
## Miro/src/pioneerBase/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


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
