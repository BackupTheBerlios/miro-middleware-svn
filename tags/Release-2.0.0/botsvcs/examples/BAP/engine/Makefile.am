## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/botsvcs/examples/BAP/engine/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


if COND_B21
b21exec = \
	B21BehaviourEngine
endif

noinst_PROGRAMS = \
	SimpleBehaviourEngine \
	RangeSensorBehaviourEngine \
	$(b21exec)


SimpleBehaviourEngine_SOURCES = SimpleBehaviourEngine.cpp
RangeSensorBehaviourEngine_SOURCES = RangeSensorBehaviourEngine.cpp
B21BehaviourEngine_SOURCES = B21BehaviourEngine.cpp

globallibs = \
	-L../simple \
	-lmiroBehavioursSimple \
	-L../rangeSensor \
	-lmiroBehavioursRangeSensor \
	-lmiroBAP

if COND_B21
b21libs = \
	-L../b21 \
	-lmiroBehavioursB21
endif

linkwithlibraries = $(globallibs) $(b21libs)
SimpleBehaviourEngine_LDADD = $(linkwithlibraries)
RangeSensorBehaviourEngine_LDADD = $(linkwithlibraries)
B21BehaviourEngine_LDADD = $(linkwithlibraries)
