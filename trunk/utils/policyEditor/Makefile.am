## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/policyEditor/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = PolicyEditor


sources = \
	PolicyDocument.cpp \
	PolicyDocument.h \
	PolicyPattern.cpp \
	PolicyPattern.h \
	PolicyBehaviour.cpp \
	PolicyBehaviour.h \
	PolicyConfig.cpp \
	PolicyConfig.h \
	PatternWidget.h \
	BehaviourWidget.h \
	ArbiterWidget.h \
	ParameterDialog.h \
	PolicyEditor.h \
	PolicyView.h \
	PolicyArbiter.h \
	Transition.h

tomocsources = \
	PolicyView.cpp \
	PatternWidget.cpp \
	BehaviourWidget.cpp \
	ArbiterWidget.cpp \
	ParameterDialog.cpp \
	PolicyEditor.cpp

PolicyEditor_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

PolicyEditor_LDADD = \
	-lTAO \
	-lTAO_CosNaming \
	-L../widgets/ -lmiroWidgets \
	-lmiroParams \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
