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
	DummyBehaviour.h \
	DummyBehaviourB.cpp \
	DummyBehaviourB.h \
	DummyArbiter.cpp \
	DummyArbiter.h \
	DummyArbiterMessage.cpp \
	DummyArbiterMessage.h \
	Parameters.xml

PolicyParsingTest_SOURCES = \
	PolicyParsingTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviour.h \
	DummyBehaviourB.cpp \
	DummyBehaviourB.h \
	DummyArbiter.cpp \
	DummyArbiter.h \
	DummyArbiterMessage.cpp \
	DummyArbiterMessage.h \
	Parameters.xml

BehaviourEngineTest_SOURCES = \
	BehaviourEngineTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviour.h \
	DummyBehaviourB.cpp \
	DummyBehaviourB.h \
	DummyArbiter.cpp \
	DummyArbiter.h \
	DummyArbiterMessage.cpp \
	DummyArbiterMessage.h \
	Parameters.xml

DynamicPolicyTest_SOURCES = \
	DynamicPolicyTest.cpp \
	DummyBehaviour.cpp \
	DummyBehaviour.h \
	DummyBehaviourB.cpp \
	DummyBehaviourB.h \
	DummyBehaviourD.cpp \
	DummyBehaviourD.h \
	DummyArbiter.cpp \
	DummyArbiter.h \
	DummyArbiterMessage.cpp \
	DummyArbiterMessage.h \
	Parameters.xml

linkwithlibraries = \
	$(top_builddir)/lib/libmiroSvc.a \
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
