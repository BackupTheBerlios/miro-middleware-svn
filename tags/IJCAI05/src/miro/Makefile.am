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
## Miro/src/miro/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules

lib_LTLIBRARIES = libmiro.la
lib_LIBRARIES = libmiroSvc.a libmiroSsh.a


sources1 = \
	Parameters.cpp \
	XmlParse.cpp \
	Exception.cpp \
	Angle.cpp \
	Configuration.cpp \
	ConfigDocument.cpp \
	Log.cpp \
	Thread.cpp \
	Task.cpp \
	ReactorTask.cpp \
	Client.cpp \
	Server.cpp \
	IO.cpp \
	TimeHelper.cpp \
	TimeSeries.cpp \
	VideoHelper.cpp \
	ScanDescriptionHelper.cpp \
	StructuredPushSupplier.cpp \
	StructuredPushConsumer.cpp \
	OdometryTracking.cpp 

Parameters.cpp: Parameters.xml
NotifyMulticastParameters.cpp: NotifyMulticastParameters.xml

libmiro_la_SOURCES = $(sources1) $(sparrowsources1_used) $(pioneersources1_used)
if COND_PCH
libmiro_la_CPPFLAGS = -include $(top_builddir)/pch/all_shared.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif
libmiro_la_LIBADD = \
	$(ACE_LIBS) \
	$(TAO_LIBS) \
        -lmiroIDL \
        $(INHERITED_LIBS) \
        $(QT_LIBS)



sources2 = \
	SvcParameters.cpp \
	DevConnection.cpp \
	DevEventHandler.cpp \
	DevConsumer.cpp \
	TtyConnection.cpp \
	LogNotifyConsumer.cpp \
	LogHeader.cpp \
	LogTypeRepository.cpp \
	LogReader.cpp \
	LogWriter.cpp \
	ObjectVectorImpl.cpp \
	RangeSensorImpl.cpp \
	OdometryImpl.cpp \
	MotionImpl.cpp \
	DifferentialMotionImpl.cpp \
	BatteryImpl.cpp \
	CompassImpl.cpp \
	InclinometerImpl.cpp \
	MagnetometerImpl.cpp \
	ThermometerImpl.cpp \
	PanImpl.cpp \
	TiltImpl.cpp \
	PanTiltImpl.cpp \
	CameraControlImpl.cpp \
	NotifyMulticastParameters.cpp \
        NotifyMulticastRequestEntry.cpp \
        NotifyMulticastSender.cpp \
        NotifyMulticastReceiver.cpp \
        NotifyMulticastTimeoutHandler.cpp \
        NotifyMulticastEventHandler.cpp \
        NotifyMulticastAdapter.cpp \
        NotifyMulticastSH.cpp \
        NotifyMulticastDomainEventFilter.cpp

SvcParameters.cpp: SvcParameters.xml

libmiroSvc_a_SOURCES = $(sources2)
if COND_PCH
libmiroSvc_a_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif


sources3 = \
	Ssh.cpp

libmiroSsh_a_SOURCES = $(sources3)
if COND_PCH
libmiroSsh_a_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

extraheader = \
	Repository.h \
	Factory.h \
	Singleton.h \
	Repository.h \
	Factory.h \
	Synch.h \
	DevMessage.h \
	NotifyMulticastRequestIndex.h \
	NotifyMulticastDefines.h \
	NotifyMulticast.h \
	Utils.h 

BUILT_SOURCES = \
	Parameters.cpp Parameters.h \
	NotifyMulticastParameters.cpp NotifyMulticastParameters.h \
	SvcParameters.cpp SvcParameters.h

CLEANFILES = $(BUILT_SOURCES)

includefiles1 = \
	$(sources1:.xml=.h) \
	$(sources2:.xml=.h) \
	$(sources3:.xml=.h) \
	$(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
miro_include_HEADERS = $(includefiles2)

all-local: libmiro.la libmiroSvc.a libmiroSsh.a
	$(INSTALLLIBRARIES)
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
	$(CLEANLIBRARIES)
