## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/video/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LTLIBRARIES = libmiroVideo.la
bin_PROGRAMS = VideoService


sources = \
	Parameters.xml \
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
	VideoServer.cpp

if COND_BTTV
bttvsources = \
	BufferManagerBTTV.cpp \
	VideoDeviceBTTV.cpp
endif

if COND_METEOR
meteorsources = \
	BufferManagerMeteor.cpp \
	VideoDeviceMeteor.cpp
endif

if COND_IEEE1394
1394sources = \
	BufferManager1394.cpp \
	VideoDevice1394.cpp \
	VideoFilterYUVbase.cpp \
	VideoFilterYUV411toRGB.cpp \
	VideoFilterYUV411toYUV.cpp \
	VideoFilterYUV422toRGB.cpp \
	VideoFilterYUV422toYUV.cpp
endif 

libmiroVideo_la_SOURCES = \
	$(sources) \
	$(bttvsources) \
	$(meteorsources) \
	$(1394sources)




VideoService_SOURCES = VideoService.cpp

if COND_IEEE1394
1394libraries = \
	-lraw1394 \
	-ldc1394_control
endif 

VideoService_LDADD = \
	-lmiroVideo \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS) \
	$(1394libraries)




extraheader = \
	VideoFilterFactory.h \
	VideoFilterLink.h \
	VideoBrokerLink.h \
	ioctl_meteor.h

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h) $(bttvsources:.xml=.h) $(meteorsources:.xml=.h) $(1394sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
miro_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLTLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLTLIBRARIES)
	$(CLEANPROGRAMS)
