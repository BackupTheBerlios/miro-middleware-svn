## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/botsvcs/examples/behaviours/rangeSensors/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


lib_LTLIBRARIES = libmiroBehavioursRangeSensor.la


libmiroBehavioursRangeSensor_la_SOURCES = \
	Parameters.xml \
	RangeSensorBehaviour.cpp \
	RangeSensorBehaviour.h \
	RangeSensorAvoid.cpp \
	RangeSensorAvoid.h \
	RangeSensorWallFollow.cpp \
	RangeSensorWallFollow.h \
	BehavioursFactory.cpp \
	BehavioursFactory.h

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
