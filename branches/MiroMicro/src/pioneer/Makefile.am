## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/pioneer/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


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
	PioneerChargerImpl.cpp \
	PioneerChargerImpl.h \
	CameraMessage.cpp \
	CameraMessage.h \
	CanonPanTiltImpl.cpp \
	CanonPanTiltImpl.h \
	CanonCameraControlImpl.cpp \
	CanonCameraControlImpl.h \
	CanonMessage.cpp \
	CanonMessage.h \
	SonyPanTiltImpl.cpp \
	SonyPanTiltImpl.h \
	SonyCameraControlImpl.cpp \
	SonyCameraControlImpl.h \
	SonyMessage.cpp \
	SonyMessage.h \
	GripperImpl.cpp \
	GripperImpl.h \
	CanonDevice.h \
	TCM2Impl.cpp \
	TCM2Impl.h

deprecatedsources = \
	CanonCameraImpl.cpp \
	CanonCameraImpl.h

Parameters.h: Parameters.cpp

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
