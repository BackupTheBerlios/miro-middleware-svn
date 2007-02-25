## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/Makefile.am
##
##############################################################################


globaldirs = \
	params \
	makeParams \
	miro \
	video \
	laser \
	laser360 \
	gps \
	fusedRangeSensor

if COND_B21
b21dirs = \
	abus \
	mcp \
	msp \
	base \
	buttons \
	b21Base \
	panTilt \
	speech
endif

if COND_SPEECH
speechdirs = \
	sphinx
endif

if COND_PIONEER
pioneerdirs = \
	psos \
	pioneer \
	pioneerBase
endif

if COND_SPARROW99
sparrowdirs = \
	can \
	sparrow \
	faulTty \
	faulMotor \
	sparrowBase
endif

if COND_PLAYER
playerdirs = \
	playerBase
endif

SUBDIRS = $(globaldirs) $(b21dirs) $(speechdirs) $(pioneerdirs) $(sparrowdirs) $(playerdirs)


