## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/video/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
AM_CXXFLAGS += $(PLAYER_CPPFLAGS)


lib_LTLIBRARIES = libmiroVideo.la
bin_PROGRAMS = VideoService


sources = \
	Parameters.cpp \
	AsynchLinkManager.cpp \
	AsynchBufferSet.cpp \
	AsynchPreLink.cpp \
	AsynchSuccLink.cpp \
	DeviceAsynchLinkManager.cpp \
	DeviceAsynchBufferSet.cpp \
	BufferManager.cpp \
	VideoFilter.cpp \
	VideoFilterBasic.cpp \
	VideoFilterRepository.cpp \
	VideoDevice.cpp \
	VideoAVDevice.cpp \
	VideoDeviceDummy.cpp \
	VideoConsumer.cpp \
	VideoImpl.cpp \
	VideoBrokerImpl.cpp \
	VideoServer.cpp \
	VideoFilterService.cpp \
	VideoFilterYUVbase.cpp \
	VideoFilterYUV411toRGB.cpp \
	VideoFilterYUV411toYUV.cpp \
	VideoFilterYUV422toRGB.cpp \
	VideoFilterYUV422toYUV.cpp \
	VideoFilterYUV420PtoRGB.cpp \
	VideoFilterOmni2Pan.cpp \
	VideoFilterRGBtoSeg.cpp \
	VideoFilterDistanceScan.cpp

playersources = \
	VideoDevicePlayer.cpp

bttvsources = \
	BufferManagerBTTV.cpp \
	VideoDeviceBTTV.cpp

meteorsources = \
	BufferManagerMeteor.cpp \
	VideoDeviceMeteor.cpp

1394sources = \
	BufferManager1394.cpp \
	VideoDevice1394.cpp

unicapsources = \
	BufferManagerUnicap.cpp \
	VideoDeviceUnicap.cpp

quickcamsources = \
	BufferManagerQuickCam.cpp \
	VideoDeviceQuickCam.cpp

if COND_PLAYER
playersources_used = $(playersources)
endif

if COND_BTTV
bttvsources_used = $(bttvsources)
endif

if COND_METEOR
meteorsources_used = $(meteorsources)
endif

if COND_IEEE1394
1394sources_used = $(1394sources)
endif

if COND_UNICAP
unicapsources_used = $(unicapsources)
endif

if COND_QUICKCAM
quickcamsources_used = $(quickcamsources)
endif

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml


libmiroVideo_la_LIBADD = $(playerlibraries)

libmiroVideo_la_SOURCES = \
	$(sources) \
	$(bttvsources_used) \
	$(meteorsources_used) \
	$(1394sources_used) \
	$(unicapsources_used) \
	$(quickcamsources_used) \
	$(playersources_used)

if COND_PCH
libmiroVideo_la_CPPFLAGS = -include $(top_builddir)/pch/all_shared.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif


VideoService_SOURCES = VideoService.cpp
if COND_PCH
VideoService_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

## local version of libmiroVideo, because it is not installed at this point.
VideoService_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	./libmiroVideo.la \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS) \
	$(1394libraries) \
	$(unicaplibraries) \
	$(playerlibraries)


extraheader = \
	VideoFilterFactory.h \
	VideoFilterLink.h \
	VideoBrokerLink.h \
	ioctl_meteor.h \
	pwc-ioctl.h \
	VideoFilter.i

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = \
	$(sources:.xml=.h) \
	$(bttvsources:.xml=.h) \
	$(meteorsources:.xml=.h) \
	$(1394sources:.xml=.h) \
	$(unicapsources:.xml=.h) \
	$(quickcamsources:.xml=.h) \
	$(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
video_include_HEADERS = $(includefiles2)

all-local: libmiroVideo.la VideoService
	$(INSTALLLTLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLTLIBRARIES)
	$(CLEANPROGRAMS)
