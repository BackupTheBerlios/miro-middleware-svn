## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/Makefile
##
##############################################################################

globaldirs = \
	config \
	odometry \
	motion \
	rangeSensor \
	fusedRangeSensor \
	panTilt \
	cameraControl \
	video \
	videoBroker \
	notify \
	notifyMulticast \
	speech \
        log

#	xml

if COND_B21
b21dirs = \
	b21 \
	dpPanTilt \
	dtlk 
endif

if COND_SPEECH
speechdirs = \
	sphinx
endif

if COND_PIONEER
pioneerdirs = \
	pioneer \
	gripper
endif

if COND_SPARROW99
sparrowdirs = \
	sparrow \
	kicker \
	faul
endif

if COND_DEPRECATED
deprecateddirs = 
endif



SUBDIRS = $(globaldirs) $(b21dirs) $(speechdirs) $(pioneerdirs) $(sparrowdirs) $(deprecateddirs)
