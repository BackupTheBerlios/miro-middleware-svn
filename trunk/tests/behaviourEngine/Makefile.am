## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/behaviourEngine/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = BehaviourEngineTest


BehaviourEngineTest_SOURCES = BehaviourEngineTest.cpp

BehaviourEngineTest_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: BehaviourEngineTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
