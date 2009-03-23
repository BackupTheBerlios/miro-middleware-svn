## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/sparrowBase/Makefile.am
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
