## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/logPlayer/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = LogPlayer


sources = \
	LogPlayer.cpp \
	ChannelManager.cpp \
	ChannelManager.h \
	LogFile.h \
	FileSet.h \
	MainForm.h

tomocsources = \
	LogFile.cpp \
	FileSet.cpp \
	EventView.cpp \
	MainForm.cpp


LogPlayer_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

LogPlayer_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroWidgets \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
