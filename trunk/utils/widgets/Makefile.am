## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/widgets/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


lib_LTLIBRARIES = libmiroWidgets.la


normalsources = \
	ConfigFile.h \
	ConfigFile.cpp \
	Item.h \
	ItemXML.h \
	DocumentXML.h \
	ParameterXML.h \
	SimpleParameter.h \
	ParameterList.h \
	CompoundParameter.h \
	FileListDialog.h \
	Validators.h \
	ParameterEdit.h \
	SimpleParameterEdit.h \
	DeferredParameterEdit.h \
	OkCancelDialog.h \
	DialogXML.h \
	ParameterDialog.h \
	ParameterListDialog.h \
	SingleParameterDialog.h

tomocsources = \
	Item.cpp \
	ItemXML.cpp \
	DocumentXML.cpp \
	DocumentView.cpp \
	ParameterXML.cpp \
	SimpleParameter.cpp \
	ParameterList.cpp \
	CompoundParameter.cpp \
	FileListDialog.cpp \
	Validators.cpp \
	ParameterEdit.cpp \
	SimpleParameterEdit.cpp \
	DeferredParameterEdit.cpp \
	OkCancelDialog.cpp \
	DialogXML.cpp \
	ParameterDialog.cpp \
	ParameterListDialog.cpp \
	SingleParameterDialog.cpp

libmiroWidgets_la_SOURCES = \
	$(normalsources) \
	$(tomocsources) \
	$(tomocsources:.cpp=.moc.cpp) \
	$(normalsources:.cpp=.h) \
	$(tomocsources:.cpp=.h)

libmiroWidgets_la_LIBADD = \
	-lmiroParams \
	-lmiro

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(tomocsources:.cpp=.moc.cpp)

all-local: libmiroWidgets.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
