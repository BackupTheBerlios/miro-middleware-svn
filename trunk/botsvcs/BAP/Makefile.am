## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/BAP/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif

lib_LTLIBRARIES = libmiroBAP.la

sources = \
	Parameters.xml \
	Pattern.cpp \
	ActionPattern.cpp \
	Arbiter.cpp \
	ArbiterParameters.cpp \
	ArbiterMessage.cpp \
	PriorityArbiter.cpp \
	MotionArbiter.cpp \
	MotionArbiterMessage.cpp \
	ConstraintArbiter.cpp \
	ConstraintArbiterParameters.cpp \
	ConstraintArbiterViewer.cpp \
	ConstraintArbiterViewerTask.cpp \
	Behaviour.cpp \
	BehaviourParameters.cpp \
	BAFactory.cpp \
	EventBehaviour.cpp \
	TaskBehaviour.cpp \
	TimedBehaviour.cpp \
	Policy.cpp \
	VelocitySpace.cpp \
	BehaviourEngineImpl.cpp

sparrowsources = \
	PowerArbiter.cpp \
	PowerArbiterMessage.cpp

Parameters.h: Parameters.cpp

if COND_SPARROW99
sparrowsources_used = $(sparrowsources)
endif

libmiroBAP_la_SOURCES = $(sources) $(sparrowsources_used)

libmiroBAP_la_LIBADD = \
	-lmiro \
	$(INHERITED_LIBS) \
	$(QT_LIBS)

extraheader = \
	Transition.h \
	BehaviourRepository.h \
	ArbiterRepository.h

BUILT_SOURCES = \
	Parameters.cpp Parameters.h

CLEANFILES = $(BUILT_SOURCES)

includefiles1 = \
	$(sources:.xml=.h) \
	$(sparrowsources:.xml=.h) \
	$(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
botsvcsBAP_include_HEADERS = $(includefiles2)

all-local: libmiroBAP.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
