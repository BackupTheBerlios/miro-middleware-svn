## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/templates/Makefile.am.qt
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@

## program to be built
bin_PROGRAMS = ConfigEditor

## sourcefiles needed for the binary to be built
sources = \
	ConfigEditor.cpp \
	MainWindow.h \
	ConfigDocumentXML.h \
	Section.h \
	ParameterInstance.h \
	ParameterSingleton.h

## sourcefiles that need to be compiled with qt's moc compiler also
## (do NOT write header files for the qt-files here, but in the variable above)
tomocsources = \
	MainWindow.cpp \
	ConfigDocumentXML.cpp \
	Section.cpp \
	ParameterInstance.cpp \
	ParameterSingleton.cpp

## the prefix of the two following macros must have the same name as the respective binary
ConfigEditor_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)
## you may add libraries to be linked here
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
