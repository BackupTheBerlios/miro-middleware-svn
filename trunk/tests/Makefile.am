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
	odometry \
	motion \
	rangeSensor \
	panTilt \
	kicker \
	video \
	videoBroker \
	notify \
	notifyMulticast \
	speech \
	behaviours \
	behaviourEngine \
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
	faul
endif

if COND_DEPRECATED
deprecateddirs = \
	image
endif



SUBDIRS = $(globaldirs) $(b21dirs) $(speechdirs) $(pioneerdirs) $(sparrowdirs) $(deprecateddirs)
