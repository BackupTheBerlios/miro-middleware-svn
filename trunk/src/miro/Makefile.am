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
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LTLIBRARIES = libmiro.la
lib_LIBRARIES = libmiroSvc.a libmiroSsh.a


sources1 = \
	XmlParse.cpp \
	Exception.cpp \
	Angle.cpp \
	ConfigDocument.cpp \
	Log.cpp \
	Thread.cpp \
	Task.cpp \
	ReactorTask.cpp \
	Client.cpp \
	Server.cpp \
	IO.cpp \
	TimeHelper.cpp \
	VideoHelper.cpp \
	ScanDescriptionHelper.cpp \
	StructuredPushSupplier.cpp \
	StructuredPushConsumer.cpp \
	OdometryTracking.cpp \
	ActionPattern.cpp \
	Arbiter.cpp \
	ArbiterParameters.cpp \
	ArbiterMessage.cpp \
	ArbiterRepository.cpp \
	PriorityArbiter.cpp \
	MotionArbiter.cpp \
	MotionArbiterMessage.cpp \
	WindowArbiter.cpp \
	WindowArbiterViewer.cpp \
	WindowArbiterViewerTask.cpp \
	Behaviour.cpp \
	BehaviourParameters.cpp \
	BehaviourRepository.cpp \
	BAFactory.cpp \
	EventBehaviour.cpp \
	TaskBehaviour.cpp \
	TimedBehaviour.cpp \
	Policy.cpp \
	DynamicWindow.cpp \
	NotifyMulticastParameters.xml \
	NotifyMulticastRequestEntry.cpp \
	NotifyMulticastSender.cpp \
        NotifyMulticastReceiver.cpp \
        NotifyMulticastTimeoutHandler.cpp \
        NotifyMulticastEventHandler.cpp \
        NotifyMulticastConfig.cpp \
	NotifyMulticastAdapter.cpp

sparrowsources1 = \
	PowerArbiter.cpp \
	PowerArbiterMessage.cpp

if COND_SPARROW99
sparrowsources1_used = $(sparrowsources1)
endif

libmiro_la_SOURCES = $(sources1) $(sparrowsources1_used)

libmiro_la_LIBADD = \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	-lTAO \
	-lACE \
	-lTAO_Strategies \
	$(INHERITED_LIBS) \
	$(QT_LIBS)



sources2 = \
	Parameters.xml \
	DevConnection.cpp \
	DevEventHandler.cpp \
	DevConsumer.cpp \
	TtyConnection.cpp \
	RangeSensorImpl.cpp \
	OdometryImpl.cpp \
	MotionImpl.cpp \
	DifferentialMotionImpl.cpp \
	BatteryImpl.cpp \
	BehaviourEngineImpl.cpp

libmiroSvc_a_SOURCES = $(sources2)



sources3 = \
	Ssh.cpp

libmiroSsh_a_SOURCES = $(sources3)



extraheader = \
	Singleton.h \
	Synch.h \
	DevMessage.h \
	NotifyMulticastRequestIndex.h \
	NotifyMulticastDefines.h \
	NotifyMulticast.h \
	Utils.h \
	TimeSeries.h

BUILT_SOURCES = \
	Parameters.cpp Parameters.h \
	NotifyMulticastParameters.cpp NotifyMulticastParameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = \
	$(sources1:.xml=.h) \
	$(sparrowsources1:.xml=.h) \
	$(sources2:.xml=.h) \
	$(sources3:.xml=.h) \
	$(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
miro_include_HEADERS = $(includefiles2)

all-local:
	$(INSTALLLIBRARIES)
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
	$(CLEANLIBRARIES)
