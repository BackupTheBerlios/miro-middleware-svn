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
## Miro/tests/videoBroker/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = VideoBrokerTest VideoBrokerTestBrief


VideoBrokerTest_SOURCES = VideoBrokerTest.cpp
VideoBrokerTest_LDADD = \
	-lmiro

VideoBrokerTestBrief_SOURCES = VideoBrokerTestBrief.cpp
VideoBrokerTestBrief_LDADD = \
	-lmiro

all-local: VideoBrokerTest VideoBrokerTestBrief
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
