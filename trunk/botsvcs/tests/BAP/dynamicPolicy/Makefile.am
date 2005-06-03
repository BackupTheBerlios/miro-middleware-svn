## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/tests/BAP/dynamicPolicy/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

bin_PROGRAMS = \
	DynamicPolicyTest

DynamicPolicyTest_SOURCES = \
	DynamicPolicyTest.cpp

DynamicPolicyTest_LDADD = \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES = 
CLEANFILES = $(BUILT_SOURCES)

all-local: DynamicPolicyTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
