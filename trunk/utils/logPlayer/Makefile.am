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
	ChannelManager.h

tomocsources = \
	LogFile.cpp \
	LogFile.h \
	FileSet.cpp \
	FileSet.h \
	MainForm.cpp \
	MainForm.h

LogPlayer_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

LogPlayer_LDADD = \
	-L../widgets/ -lmiroWidgets \
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
