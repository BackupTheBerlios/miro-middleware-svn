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
	$(QT_BINDIR)/moc $< -o $@


bin_PROGRAMS = PolicyEditor


sources = \
	PolicyDocument.cpp \
	PolicyPattern.cpp \
	PolicyBehaviour.cpp \
	PolicyConfig.cpp

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
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(tomocsources:.cpp=.moc.cpp)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
