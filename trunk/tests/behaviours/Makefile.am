## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/behaviours/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	PriorityArbitersTest \
	PolicyParsingTest \
	BehaviourEngineTest \
	DynamicPolicyTest


PriorityArbitersTest_SOURCES = \
	PriorityArbitersTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviourB.cpp \
	DummyArbiter.cpp \
	DummyArbiterMessage.cpp \
	Parameters.xml

PolicyParsingTest_SOURCES = \
	PolicyParsingTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviourB.cpp \
	DummyArbiter.cpp \
	DummyArbiterMessage.cpp \
	Parameters.xml

BehaviourEngineTest_SOURCES = \
	BehaviourEngineTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviourB.cpp \
	DummyArbiter.cpp \
	DummyArbiterMessage.cpp \
	Parameters.xml

DynamicPolicyTest_SOURCES = \
	DynamicPolicyTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviourB.cpp \
	DummyBehaviourD.cpp \
	DummyArbiter.cpp \
	DummyArbiterMessage.cpp \
	Parameters.xml

linkwithlibraries = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

PriorityArbitersTest_LDADD = $(linkwithlibraries)
PolicyParsingTest_LDADD = $(linkwithlibraries)
BehaviourEngineTest_LDADD = $(linkwithlibraries)
DynamicPolicyTest_LDADD = $(linkwithlibraries)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
