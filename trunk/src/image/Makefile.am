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
	Pixels.cpp \
	FrameGrabber.cpp \
	FrameGrabberMeteor.cpp \
	ImageSource.cpp \
	VideoSource.cpp \
	PatternSource.cpp \
	ImageFactoryDefinitions.cpp \
	ImageFactory.cpp \
	ImageServer.cpp \
	ImageImpl.cpp

MiroImage_LDADD = \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	-lTAO \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
