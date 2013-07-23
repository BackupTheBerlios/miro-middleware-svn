## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2006
## Institute for Softwaretechnology, Technical University Graz, Austria
##
## $Id$
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

bin_PROGRAMS = InertialService

InertialService_SOURCES = \
	Parameters.xml \
	InertialSensorImpl.h \
	InertialSensorImpl.cpp \
	XsensInertialSensorMessage.h \
	XsensInertialSensorConnection.h \
	XsensInertialSensorConnection.cpp \
	Xbus.h \
	Xbus.cpp \
	InertialSensorPollTask.h \
	InertialSensorPollTask.cpp \
	InertialSensorEventHandler.h \
	InertialSensorEventHandler.cpp \
	InertialService.cpp \
	InertialService.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

InertialService_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local: InertialService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
