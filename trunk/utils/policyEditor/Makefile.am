## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/policyEditor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = PolicyEditor


sources = \
	PolicyEditor.cpp \
	PolicyDocumentXML.h \
	PatternXML.h \
	PolicyXML.h \
	SubpolicyXML.h \
	ActionPatternXML.h \
	BehaviourXML.h \
	ArbiterXML.h \
	TransitionXML.h \
	DownTransitionXML.h \
	ExternalTransitionXML.h \
	MainWindow.h \
	ParameterWidget.h \
	BehaviourWidget.h \
	ArbiterWidget.h \
	PatternWidget.h \
	PolicyView.h \
	TransitionContainer.h \
	TransitionWidget.h \
	Transition.h \
	TransitionDialog.h

tomocsources = \
	PolicyDocumentXML.cpp \
	PatternXML.cpp \
	PolicyXML.cpp \
	SubpolicyXML.cpp \
	ActionPatternXML.cpp \
	BehaviourXML.cpp \
	ArbiterXML.cpp \
	TransitionXML.cpp \
	DownTransitionXML.cpp \
	ExternalTransitionXML.cpp \
	MainWindow.cpp \
	ParameterWidget.cpp \
	BehaviourWidget.cpp \
	ArbiterWidget.cpp \
	PatternWidget.cpp \
	PolicyView.cpp \
	TransitionContainer.cpp \
	TransitionWidget.cpp  \
	TransitionDialog.cpp


PolicyEditor_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

PolicyEditor_LDADD = \
	-lmiroWidgets \
	-lmiroParams \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: PolicyEditor
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

