## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/examples/behaviours/b21/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


noinst_LTLIBRARIES = libmiroBehavioursB21.la


libmiroBehavioursB21_la_SOURCES = \
	Parameters.xml \
	LaserAvoid.cpp \
	LaserAvoid.h \
	SonarAvoid.cpp \
	SonarAvoid.h \
	OnButton.cpp \
	OnButton.h \
	BehavioursFactory.cpp \
	BehavioursFactory.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: libmiroBehavioursB21.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
