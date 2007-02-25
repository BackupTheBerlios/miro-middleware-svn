## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/params/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


noinst_PROGRAMS = ParameterExample


ParameterExample_SOURCES = \
	ParameterExample.cpp \
	Parameters.xml

Parameters.h: Parameters.cpp

ParameterExample_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: ParameterExample
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
