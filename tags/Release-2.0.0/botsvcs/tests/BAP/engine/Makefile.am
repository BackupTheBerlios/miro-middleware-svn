## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/tests/BAP/engine/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

noinst_PROGRAMS = \
	BehaviourEngineTest DummyBehaviourEngine

BehaviourEngineTest_SOURCES = \
	BehaviourEngineTest.cpp

DummyBehaviourEngine_SOURCES = \
	DummyBehaviourEngine.cpp

BehaviourEngineTest_LDADD = \
	-L../lib \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiro

DummyBehaviourEngine_LDADD = \
	-L../lib \
	-lmiroBAPtests \
	-lmiroBAP \
	-lmiro

BUILT_SOURCES = 
CLEANFILES = $(BUILT_SOURCES)

all-local: BehaviourEngineTest DummyBehaviourEngine
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
