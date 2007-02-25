## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/robotikI/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


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
	-lmiro

LaserGet_LDADD = $(linkwithlibraries)
LaserStream_LDADD = $(linkwithlibraries)
SonarGet_LDADD = $(linkwithlibraries)
SonarStream_LDADD = $(linkwithlibraries)
SonarStream2_LDADD = $(linkwithlibraries)

all-local: LaserGet LaserStream SonarGet SonarStream SonarStream2
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
