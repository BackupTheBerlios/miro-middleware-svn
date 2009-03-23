## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/sonar/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	SonarPoll1 \
	SonarPoll2 \
	SonarPoll3 \
	SonarNotify


SonarPoll1_SOURCES = SonarPoll1.cpp
SonarPoll2_SOURCES = SonarPoll2.cpp
SonarPoll3_SOURCES = SonarPoll3.cpp
SonarNotify_SOURCES = SonarNotify.cpp

linkwithlibraries = \
	-lmiro

SonarPoll1_LDADD = $(linkwithlibraries)
SonarPoll2_LDADD = $(linkwithlibraries)
SonarPoll3_LDADD = $(linkwithlibraries)
SonarNotify_LDADD = $(linkwithlibraries)

all-local: SonarPoll1 SonarPoll2 SonarPoll3 SonarNotify
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
