## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/odometry/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
