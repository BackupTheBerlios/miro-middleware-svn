## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/Makefile.am
##
##############################################################################


globaldirs = \
	nsIOR \
	widgets \
	notify \
	notifyObserver \
	logPlayer \
	rangeSensor \
	qtVideo \
	qtTouchPad \
	qtJoystick \
        qtCharger \
	configEditor \
	policyController \
	policyEditor

#	makeConfig
#	sparrowBoard

if COND_SPARROW99
sparrowdirs = \
	Joystick
endif



SUBDIRS = $(globaldirs) $(sparrowdirs)
