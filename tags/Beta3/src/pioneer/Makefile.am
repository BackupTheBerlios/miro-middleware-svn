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
## Miro/src/pioneer/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LIBRARIES = libpioneer.a


sources = \
	Parameters.xml \
	PioneerConnection.cpp \
	PioneerConnection.h \
	PioneerConsumer.cpp \
	PioneerConsumer.h \
	PioneerMotionImpl.cpp  \
	PioneerMotionImpl.h \
	PioneerStallImpl.cpp \
	PioneerStallImpl.h \
	CanonPanTiltImpl.cpp \
	CanonPanTiltImpl.h \
	CanonCameraImpl.cpp \
	CanonCameraImpl.h \
	CanonMessage.cpp \
	CanonMessage.h \
	GripperImpl.cpp \
	GripperImpl.h \
	CanonDevice.h

libpioneer_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
pioneer_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
