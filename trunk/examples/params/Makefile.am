## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/params/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = ParameterExample


ParameterExample_SOURCES = \
	ParameterExample.cpp \
	Parameters.xml

ParameterExample_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: ParameterExample
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
