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
## Miro/performance-tests/response/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	SparrowResponse \
	PollResponse \
	NotifyResponse


SparrowResponse_SOURCES = SparrowResponse.cpp
PollResponse_SOURCES = PollResponse.cpp
NotifyResponse_SOURCES = NotifyResponse.cpp

linkwithlibraries = \
	-lTAO \
	-lTAO_CosNaming \
	-lsparrow \
	-lcan \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

SparrowResponse_LDADD = $(linkwithlibraries)
PollResponse_LDADD = $(linkwithlibraries)
NotifyResponse_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
