## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2005 
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

bin_PROGRAMS = GpsService

GpsService_SOURCES = \
	Parameters.xml \
	CtGpsImpl.cpp \
	CtGpsImpl.h \
	GpsConnection.cpp \
	GpsConnection.h \
	GpsEventHandler.cpp \
	GpsEventHandler.h \
	GpsImpl.cpp \
	GpsImpl.h \
	GpsMessage.cpp \
	GpsMessage.h \
	GpsService.cpp \
	GpsService.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

GpsService_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local: GpsService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
