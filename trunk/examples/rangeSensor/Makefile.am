## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules


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

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
