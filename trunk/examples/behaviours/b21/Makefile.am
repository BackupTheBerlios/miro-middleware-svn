## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.lib
##
##############################################################################

include $(top_srcdir)/config/Make-rules
# AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`


lib_LTLIBRARIES = libmiroBehavioursB21.la


libmiroBehavioursB21_la_SOURCES = \
	Parameters.xml \
	LaserAvoid.cpp \
	SonarAvoid.cpp \
	OnButton.cpp \
	BehavioursFactory.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)
