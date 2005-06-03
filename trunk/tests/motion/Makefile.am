## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/motionMakefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	MotionTest \
	DifferentialMotionTest \
	SynchroMotionTest


MotionTest_SOURCES = \
	MotionTest.cpp \
	MotionInterface.cpp \
	MotionInterface.h

DifferentialMotionTest_SOURCES = \
	DifferentialMotionTest.cpp \
	DifferentialMotionInterface.cpp \
	DifferentialMotionInterface.h \
	MotionInterface.cpp \
	MotionInterface.h

SynchroMotionTest_SOURCES = \
	SynchroMotionTest.cpp \
	SynchroMotionInterface.cpp \
	SynchroMotionInterface.h \
	MotionInterface.cpp \
	MotionInterface.h

linkwithlibraries = \
	-lmiro

MotionTest_LDADD = $(linkwithlibraries)
DifferentialMotionTest_LDADD = $(linkwithlibraries)
SynchroMotionTest_LDADD = $(linkwithlibraries)

all-local: MotionTest DifferentialMotionTest SynchroMotionTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
