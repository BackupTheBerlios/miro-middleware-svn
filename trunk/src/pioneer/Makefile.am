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
	CanonCameraControlImpl.cpp \
	CanonCameraControlImpl.h \
	CanonMessage.cpp \
	CanonMessage.h \
	GripperImpl.cpp \
	GripperImpl.h \
	CanonDevice.h \
	TCM2Impl.cpp \
	TCM2Impl.h

deprecatedsources = \
	CanonCameraImpl.cpp \
	CanonCameraImpl.h

if COND_DEPRECATED
deprecatedsources_used = $(deprecatedsources)
endif

libpioneer_a_SOURCES = $(sources) $(deprecatedsources_used)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h) $(deprecatedsources)
includefiles2 = $(includefiles1:.cpp=.h)
pioneer_include_HEADERS = $(includefiles2)

all-local: libpioneer.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
