## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.lib
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


lib_LTLIBRARIES = libmiroBehavioursSimple.la


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

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)
