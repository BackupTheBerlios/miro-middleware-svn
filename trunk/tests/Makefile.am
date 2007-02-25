## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/Makefile.am
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

SUBDIRS = $(globaldirs) $(b21dirs) $(speechdirs) $(pioneerdirs) $(sparrowdirs)
