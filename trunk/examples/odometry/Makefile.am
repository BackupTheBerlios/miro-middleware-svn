## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/odometry/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	PollPosition \
	SetPosition \
	OdometryStream


PollPosition_SOURCES = PollPosition.cpp
SetPosition_SOURCES = SetPosition.cpp
OdometryStream_SOURCES = OdometryStream.cpp OdometryStream.h

linkwithlibraries = \
	-lTAO \
	-lTAO_CosNaming \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

PollPosition_LDADD = $(linkwithlibraries)
SetPosition_LDADD = $(linkwithlibraries)
OdometryStream_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
