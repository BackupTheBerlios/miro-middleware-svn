## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004, 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/tests/BAP/parsing/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	PolicyParsingTest


PolicyParsingTest_SOURCES = \
	PolicyParsingTest.cpp

PolicyParsingTest_LDADD = \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES =
CLEANFILES = $(BUILT_SOURCES)

all-local: PolicyParsingTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
