## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/rangeSensor/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	ScanDescription \
	SensorGet \
	SensorStream


ScanDescription_SOURCES = ScanDescription.cpp
SensorGet_SOURCES = SensorGet.cpp
SensorStream_SOURCES = SensorStream.cpp SensorStream.h

linkwithlibraries = \
	-lmiro

ScanDescription_LDADD = $(linkwithlibraries)
SensorGet_LDADD = $(linkwithlibraries)
SensorStream_LDADD = $(linkwithlibraries)

all-local: ScanDescription SensorGet SensorStream
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
