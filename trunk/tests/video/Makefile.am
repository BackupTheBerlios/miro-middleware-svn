## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
##
##
## Miro/demo/laserSaver/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


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
