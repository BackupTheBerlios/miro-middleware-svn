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
	Parameters.cpp \
	SickLaserConnection.cpp \
	SickLaserTask.cpp \
	SickLaserPollTask.cpp \
	SickLaserEvent.cpp \
	SickLaserImpl.cpp

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml

if COND_PCH
libmiroLaser_a_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif


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
if COND_PCH
SickLaserService_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

SickLaserService_LDADD = \
	libmiroLaser.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: SickLaserService libmiroLaser.a
	$(INSTALLLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLIBRARIES)
	$(CLEANPROGRAMS)
