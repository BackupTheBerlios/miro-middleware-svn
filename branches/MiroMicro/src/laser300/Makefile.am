## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id: Makefile.am,v 1.9 2007-02-25 07:09:42 hutz Exp $
##
## Miro/src/laser/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SickLaserS300Service

lib_LIBRARIES = libmiroLaserS300.a

libmiroLaserS300_a_SOURCES = \
	Parameters.cpp \
	SickLaserConnection.cpp \
	SickLaserTask.cpp \
	SickLaserEvent.cpp \
	SickLaserImpl.cpp

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml

if COND_PCH
libmiroLaserS300_a_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif


laser_include_HEADERS = \
	SickLaserConnection.h \
	SickLaserTask.h \
	SickLaserEvent.h \
	SickLaserImpl.h \
	SickLaserStatistic.h \
	Parameters.h

SickLaserS300Service_SOURCES = \
	SickLaserService.cpp \
	SickLaserService.h
if COND_PCH
SickLaserServiceS300_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

SickLaserS300Service_LDADD = \
	libmiroLaserS300.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: SickLaserS300Service libmiroLaserS300.a
	$(INSTALLLIBRARIES)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANLIBRARIES)
	$(CLEANPROGRAMS)
