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
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	PollPosition \
	SetPosition \
	OdometryStream


PollPosition_SOURCES = PollPosition.cpp
SetPosition_SOURCES = SetPosition.cpp
OdometryStream_SOURCES = OdometryStream.cpp OdometryStream.h

linkwithlibraries = \
	-lmiro

PollPosition_LDADD = $(linkwithlibraries)
SetPosition_LDADD = $(linkwithlibraries)
OdometryStream_LDADD = $(linkwithlibraries)

all-local: PollPosition SetPosition OdometryStream
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
