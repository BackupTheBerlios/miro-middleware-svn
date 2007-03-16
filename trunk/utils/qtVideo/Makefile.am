## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/qtVideo/Makefile.am
##
##############################################################################



include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

bin_PROGRAMS = QtVideo QtVideoControl

video_sources = \
	QtVideo.cpp \
	MainForm.h

video_tomocsources = \
	MainForm.cpp

control_ui = \
	VideoControlDialog.ui

control_sources = \
	VideoControl.h \
	QtVideoControl.cpp

control_tomocsources = \
        $(control_ui:.ui=.cpp) \
	VideoControl.cpp

QtVideo_SOURCES = $(video_sources) $(video_tomocsources) $(video_tomocsources:.cpp=.moc.cpp)
QtVideo_LDADD = \
	-lmiro \
	$(QT_LIBS)

QtVideoControl_SOURCES = $(control_sources) $(control_tomocsources) $(control_tomocsources:.cpp=.moc.cpp)
QtVideoControl_LDADD = \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(video_tomocsources:.cpp=.moc.cpp) $(control_tomocsources:.cpp=.moc.cpp) $(control_ui:.ui=.h)  $(control_ui:.ui=.cpp) 
CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = $(control_ui)

all-local: $(bin_PROGRAMS)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

