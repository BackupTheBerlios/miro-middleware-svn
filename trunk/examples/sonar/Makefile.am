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
## Miro/examples/sonar/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


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
	-lTAO \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

SonarPoll1_LDADD = $(linkwithlibraries)
SonarPoll2_LDADD = $(linkwithlibraries)
SonarPoll3_LDADD = $(linkwithlibraries)
SonarNotify_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
