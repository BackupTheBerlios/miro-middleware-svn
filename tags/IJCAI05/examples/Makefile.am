## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/examples/Makefile
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
