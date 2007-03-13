## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/logPlayer/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = LogPlayer time2date


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

time2date_SOURCES = time2date.cpp
time2date_LDADD = -lmiro

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: $(bin_PROGRAMS)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
