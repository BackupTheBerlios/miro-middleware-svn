## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
##
##
## Miro/demo/laserSaver/Makefile
##
##############################################################################


include $(top_srcdir)/config/Make-rules

SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@

%.h: %.ui
	$(QT_UIC) -o $@ $<

%.cpp: %.ui
	$(QT_UIC) -o $@ -impl $*.h $<


bin_PROGRAMS = QtVideo QtVideoControl VideoControlTest

video_sources = \
	QtVideo.cpp \
	MainForm.h

video_tomocsources = \
	MainForm.cpp

control_sources = \
	QtVideoControl.cpp

control_tomocsources = \
	VideoControlDialog.cpp \
	VideoControlDialogImp.cpp

QtVideo_SOURCES = $(video_sources) $(video_tomocsources) $(video_tomocsources:.cpp=.moc.cpp)
QtVideo_LDADD = \
	-lTAO \
	-lTAO_CosNaming \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

QtVideoControl_SOURCES = $(control_sources) $(control_tomocsources) $(control_tomocsources:.cpp=.moc.cpp)
QtVideoControl_LDADD = \
	-lTAO \
	-lTAO_CosNaming \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

VideoControlTest_SOURCES = VideoControlTest.cpp VideoControlTestImpl.cpp
VideoControlTest_LDADD = -lTAO $(top_builddir)/lib/libmiroSvc.a -lmiroIDL -lmiro

BUILT_SOURCES = $(video_tomocsources:.cpp=.moc.cpp) $(control_tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

VideoControlDialog.h: VideoControlDialog.ui

VideoControlDialog.cpp: VideoControlDialog.ui VideoControlDialog.h

