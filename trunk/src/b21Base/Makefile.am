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
## Miro/src/b21Base/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = B21Base B21BaseDummy


B21Base_SOURCES = B21Base.cpp B21Base.h

B21BaseDummy_SOURCES = B21BaseDummy.cpp B21BaseDummy.h

linkwithlibraries = \
	$(top_builddir)/lib/libB21Buttons.a \
	$(top_builddir)/lib/libmsp.a \
	$(top_builddir)/lib/libabus.a \
	$(top_builddir)/lib/libbase.a \
	$(top_builddir)/lib/libmcp.a \
	$(top_builddir)/lib/libmiroSvc.a \
        -lmiroIDL \
	-lmiro

B21Base_LDADD = $(linkwithlibraries)
B21BaseDummy_LDADD = $(linkwithlibraries)

all-local: B21Base B21BaseDummy
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
