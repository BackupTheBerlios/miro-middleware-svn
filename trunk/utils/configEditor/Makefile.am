## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/configEditor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = ConfigEditor

sources = \
	ConfigEditor.cpp \
	MainWindow.h \
	ConfigDocumentXML.h \
	Section.h \
	ParameterInstance.h \
	ParameterSingleton.h

tomocsources = \
	MainWindow.cpp \
	ConfigDocumentXML.cpp \
	Section.cpp \
	ParameterInstance.cpp \
	ParameterSingleton.cpp

ConfigEditor_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)
ConfigEditor_LDADD = \
	-lmiroWidgets \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: ConfigEditor
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
