## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/examples/BAP/rangeSensor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


noinst_LTLIBRARIES = libmiroBehavioursRangeSensor.la


libmiroBehavioursRangeSensor_la_SOURCES = \
	Parameters.xml \
	RangeSensorBehaviour.cpp \
	RangeSensorBehaviour.h \
	RangeSensorAvoid.cpp \
	RangeSensorAvoid.h \
	RangeSensorWallFollow.cpp \
	RangeSensorWallFollow.h \
	BehavioursFactory.cpp \
	BehavioursFactory.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: libmiroBehavioursRangeSensor.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
