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
## Miro/tests/sparrow/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	SparrowTest2003 \
	SparrowAlive \
	SparrowTest \
	PanPosition \
	KickerTest


SparrowTest2003_SOURCES = SparrowTest2003.cpp
SparrowAlive_SOURCES = SparrowAlive.cpp SparrowAlive.h
SparrowTest_SOURCES = SparrowTest.cpp
PanPosition_SOURCES = PanPosition.cpp
KickerTest_SOURCES = KickerTest.cpp

linkwithlibraries = \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

SparrowTest2003_LDADD = $(linkwithlibraries)
SparrowAlive_LDADD = $(linkwithlibraries)
SparrowTest_LDADD = $(linkwithlibraries)
PanPosition_LDADD = $(linkwithlibraries)
KickerTest_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
