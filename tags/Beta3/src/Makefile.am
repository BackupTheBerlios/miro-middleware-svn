## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/Makefile
##
##############################################################################

globaldirs = \
	params \
	makeParams \
	miro \
	video

if COND_B21
b21dirs = \
	abus \
	mcp \
	msp \
	base \
	buttons \
	b21Base \
	panTilt \
	speech \
	laser
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
if !COND_PIONEER
sparrowdirs1 = \
	psos \
	pioneer
endif 
sparrowdirs2 = \
	can \
	sparrow \
	faulTty \
	faulMotor \
	sparrowBase
endif

if COND_DEPRECATED
deprecateddirs = \
	image
endif


SUBDIRS = $(globaldirs) $(b21dirs) $(speechdirs) $(pioneerdirs) $(sparrowdirs1) $(sparrowdirs2) $(deprecateddirs)


