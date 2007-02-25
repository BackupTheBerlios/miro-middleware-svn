## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/fusedRangeSensor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = FusedRangeSensor

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml

laser_include_HEADERS = \
	FusedRangeSensorImpl.h \
	Parameters.h

FusedRangeSensor_SOURCES = \
	Parameters.cpp \
	FusedRangeSensorImpl.cpp \
	FusedRangeSensor.cpp \
	FusedRangeSensor.h

if COND_PCH
FusedRangeSensor_CPPFLAGS = -include $(top_builddir)/pch/all_static.h $(CPPFLAGS) $(AM_CPPFLAGS)
endif

FusedRangeSensor_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: FusedRangeSensor
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
