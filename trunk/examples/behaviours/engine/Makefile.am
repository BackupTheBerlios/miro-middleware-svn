## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/behaviours/engine/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


if COND_B21
b21exec = \
	B21BehaviourEngine
endif

bin_PROGRAMS = \
	SimpleBehaviourEngine \
	RangeSensorBehaviourEngine \
	$(b21exec)


SimpleBehaviourEngine_SOURCES = SimpleBehaviourEngine.cpp
RangeSensorBehaviourEngine_SOURCES = RangeSensorBehaviourEngine.cpp
B21BehaviourEngine_SOURCES = B21BehaviourEngine.cpp

globallibs = \
	$(top_builddir)/lib/libmiroSvc.a \
	-L../simple/ -lmiroBehavioursSimple \
	-L../rangeSensor/ -lmiroBehavioursRangeSensor \
	-lmiroIDL \
	-lmiro

if COND_B21
b21libs = \
	-L../b21/ -lmiroBehavioursB21
endif

linkwithlibraries = $(globallibs) $(b21libs)
SimpleBehaviourEngine_LDADD = $(linkwithlibraries)
RangeSensorBehaviourEngine_LDADD = $(linkwithlibraries)
B21BehaviourEngine_LDADD = $(linkwithlibraries)
