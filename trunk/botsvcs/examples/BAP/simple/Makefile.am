## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/examples/behaviours/simple/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


noinst_LTLIBRARIES = libmiroBehavioursSimple.la


libmiroBehavioursSimple_la_SOURCES = \
	Parameters.xml \
	TactileStop.cpp \
	TactileStop.h \
	Straight.cpp \
	Straight.h \
	Wander.cpp \
	Wander.h \
	Timer.cpp \
	Timer.h \
	MoveToPoint.cpp \
	MoveToPoint.h \
	BehavioursFactory.cpp \
	BehavioursFactory.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: libmiroBehavioursSimple.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
