## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/motion/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	MotionStatusPoll \
	MotionStatusNotify


MotionStatusPoll_SOURCES = MotionStatusPoll.cpp
MotionStatusNotify_SOURCES = MotionStatusNotify.cpp MotionStatusNotify.h

linkwithlibraries = \
	-lmiro

MotionStatusPoll_LDADD = $(linkwithlibraries)
MotionStatusNotify_LDADD = $(linkwithlibraries)

all-local: MotionStatusPoll MotionStatusNotify
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
