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
## Miro/src/image/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_CXXFLAGS += -DMIRO -DDEBUG_LOG -DSTL_SAVVY -DVIDEO_METEOR


bin_PROGRAMS = MiroImage


MiroImage_SOURCES = \
	MiroImage.cpp \
	MiroImage.h \
	Pixels.cpp \
	Pixels.h \
	pixels.ih \
	FrameGrabber.cpp \
	frameGrabber.h \
	FrameGrabberMeteor.cpp \
	frameGrabberMeteor.h \
	ImageSource.cpp \
	imageSource.h \
	VideoSource.cpp \
	videoSource.h \
	PatternSource.cpp \
	patternSource.h \
	ImageFactoryDefinitions.cpp \
	ImageFactoryDefinitions.h \
	ImageFactory.cpp \
	ImageFactory.h \
	ImageServer.cpp \
	ImageServer.h \
	ImageImpl.cpp
	ImageImpl.h \
	ioctl_meteor.h 

MiroImage_LDADD = \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	-lTAO \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
