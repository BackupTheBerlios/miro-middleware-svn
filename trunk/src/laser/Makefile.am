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
## Miro/src/laser/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SickLaserService

lib_LIBRARIES = libmiroLaser.a

libmiroLaser_a_SOURCES = \
	Parameters.xml \
	SickLaserConnection.cpp \
	SickLaserTask.cpp \
	SickLaserPollTask.cpp \
	SickLaserEvent.cpp \
	SickLaserImpl.cpp

laser_include_HEADERS = \
	SickLaserConnection.h \
	SickLaserTask.h \
	SickLaserPollTask.h \
	SickLaserEvent.h \
	SickLaserImpl.h \
	SickLaserMessage.h \
	SickLaserStatistic.h \
	SickLaserOpcodes.h \
	Parameters.h

SickLaserService_SOURCES = \
	SickLaserService.cpp \
	SickLaserService.h

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

SickLaserService_LDADD = \
	libmiroLaser.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local:
	$(INSTALLLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLIBRARIES)
	$(CLEANPROGRAMS)
