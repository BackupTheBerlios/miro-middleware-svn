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
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	SparrowResponse \
	PollResponse \
	NotifyResponse


SparrowResponse_SOURCES = SparrowResponse.cpp
PollResponse_SOURCES = PollResponse.cpp
NotifyResponse_SOURCES = NotifyResponse.cpp

linkwithlibraries = \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

SparrowResponse_LDADD = $(linkwithlibraries)
PollResponse_LDADD = $(linkwithlibraries)
NotifyResponse_LDADD = $(linkwithlibraries)

all-local: SparrowResponse PollResponse NotifyResponse
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
