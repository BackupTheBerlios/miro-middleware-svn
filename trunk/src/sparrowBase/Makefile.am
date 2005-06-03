## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/sparrowBase/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SparrowBase


SparrowBase_SOURCES = \
	SparrowServer.cpp \
	SparrowServer.h \
	SparrowBase.cpp

SparrowBase_LDADD = \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libfaulMotor.a \
	$(top_builddir)/lib/libfaulTty.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

all-local: SparrowBase
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
