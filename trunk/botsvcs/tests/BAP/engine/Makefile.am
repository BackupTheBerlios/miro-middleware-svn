## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004, 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/tests/BAP/engine/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules

bin_PROGRAMS = \
	BehaviourEngineTest DummyBehaviourEngine

BehaviourEngineTest_SOURCES = \
	BehaviourEngineTest.cpp

DummyBehaviourEngine_SOURCES = \
	DummyBehaviourEngine.cpp

BehaviourEngineTest_LDADD = \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiro

DummyBehaviourEngine_LDADD = \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiro

BUILT_SOURCES = 
CLEANFILES = $(BUILT_SOURCES)

all-local: BehaviourEngineTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
