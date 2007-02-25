## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/notifyObserver/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


## program to be built
bin_PROGRAMS = NotifyObserver

## sourcefiles needed for the binary to be built
sources = \
	NotifyObserver.cpp \
	EventTypeDialog.h \
	QoSAdmin.h \
	FilterAdmin.h \
	NotifyPublish.h \
	NotifySubscribe.h \
	EventChannelFactory.h \
	EventChannel.h \
	ConsumerAdmin.h \
	SupplierAdmin.h \
	ProxyConsumer.h \
	ProxySupplier.h \
	ApplicationWindow.h

## sourcefiles that need to be compiled with qt's moc compiler also
## (do NOT write header files for the qt-files here, but in the variable above)
tomocsources = \
	EventTypeDialog.cpp \
	QoSAdmin.cpp \
	FilterAdmin.cpp \
	NotifyPublish.cpp \
	NotifySubscribe.cpp \
	EventChannel.cpp \
	EventChannelFactory.cpp \
	ConsumerAdmin.cpp \
	SupplierAdmin.cpp \
	ProxyConsumer.cpp \
	ProxySupplier.cpp \
	ApplicationWindow.cpp

## the prefix of the two following macros must have the same name as the respective binary
NotifyObserver_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)
## you may add libraries to be linked here
NotifyObserver_LDADD = -lmiro -lmiroWidgets

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: NotifyObserver
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
