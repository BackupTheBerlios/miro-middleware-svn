## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/Makefile.am
##
##############################################################################


globaldirs = \
	odometry \
	motion \
	rangeSensor \
	buttons \
	sonar \
	stall \
	video \
	videoFilter \
	videoFilter2 \
	battery \
	notify \
	infrared \
	params \
	speechSimple \
	gps

if COND_B21
b21dirs = \
	robotikI \
	b21Motion
endif

if COND_PIONEER
pioneerdirs = \
	tcm2
endif

SUBDIRS = $(globaldirs) $(b21dirs) $(pioneerdirs)
