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


lib_LIBRARIES = libsparrowBase.a
bin_PROGRAMS = SparrowBase


libsparrowBase_a_SOURCES = \
	SparrowServer.cpp \
	SparrowServer.h

SparrowBase_SOURCES = \
	SparrowBase.cpp

SparrowBase_LDADD = \
	SparrowServer.o \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libfaulMotor.a \
	$(top_builddir)/lib/libfaulTty.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

sparrowBase_include_HEADERS = SparrowServer.h

all-local: SparrowBase libsparrowBase.a
	$(INSTALLPROGRAMS)
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANPROGRAMS)
	$(CLEANLIBRARIES)
