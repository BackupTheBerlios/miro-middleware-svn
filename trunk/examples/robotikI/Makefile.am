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
	LaserGet \
	LaserStream \
	SonarGet \
	SonarStream \
	SonarStream2


LaserGet_SOURCES = LaserGet.cpp
LaserStream_SOURCES = LaserStream.cpp
SonarGet_SOURCES = SonarGet.cpp
SonarStream_SOURCES = SonarStream.cpp
SonarStream2_SOURCES = SonarStream2.cpp SonarStream2.h

linkwithlibraries = \
	-lTAO \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

LaserGet_LDADD = $(linkwithlibraries)
LaserStream_LDADD = $(linkwithlibraries)
SonarGet_LDADD = $(linkwithlibraries)
SonarStream_LDADD = $(linkwithlibraries)
SonarStream2_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
