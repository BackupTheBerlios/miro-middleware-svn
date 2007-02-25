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


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@

%.h: %.ui
	$(QT_UIC) -o $@ $<

%.cpp: %.ui
	$(QT_UIC) -o $@ -impl $*.h $<

if COND_IEEE1394
1394programs = QtVideoControl VideoControlTest
endif

bin_PROGRAMS = QtVideo $(1394programs)

video_sources = \
	QtVideo.cpp \
	MainForm.h

video_tomocsources = \
	MainForm.cpp

control_sources = \
	VideoControlDialogImp.h \
	QtVideoControl.cpp

control_tomocsources = \
	VideoControlDialog.cpp \
	VideoControlDialogImp.cpp

QtVideo_SOURCES = $(video_sources) $(video_tomocsources) $(video_tomocsources:.cpp=.moc.cpp)
QtVideo_LDADD = \
	-lmiro \
	$(QT_LIBS)

QtVideoControl_SOURCES = $(control_sources) $(control_tomocsources) $(control_tomocsources:.cpp=.moc.cpp)
QtVideoControl_LDADD = \
	-lmiro \
	$(QT_LIBS)

VideoControlTest_SOURCES = VideoControlTest.cpp VideoControlTestImpl.cpp VideoControlTestImpl.h
VideoControlTest_LDADD = -lmiro

BUILT_SOURCES = $(video_tomocsources:.cpp=.moc.cpp) $(control_tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = VideoControlDialog.ui

all-local: QtVideo $(1394programs)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

VideoControlDialog.h: VideoControlDialog.ui

VideoControlDialog.cpp: VideoControlDialog.ui VideoControlDialog.h

