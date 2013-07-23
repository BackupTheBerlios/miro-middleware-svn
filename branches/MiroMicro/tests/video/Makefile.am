## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/video/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = TestVideoClient TestRemoteVideoClient


TestVideoClient_SOURCES = TestVideoClient.cpp
TestRemoteVideoClient_SOURCES = TestRemoteVideoClient.cpp

linkwithlibraries = \
	-lmiro

TestVideoClient_LDADD = $(linkwithlibraries)
TestRemoteVideoClient_LDADD = $(linkwithlibraries)

all-local: TestVideoClient TestRemoteVideoClient
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
