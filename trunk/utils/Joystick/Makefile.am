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
## Miro/examples/sonar/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	Joystick \
	JoystickService


Joystick_SOURCES = Joystick.cpp Joystick.h
JoystickService_SOURCES = JoystickService.cpp JoystickService.h

linkwithlibraries = \
	-lmiro

Joystick_LDADD = $(linkwithlibraries)
JoystickService_LDADD = $(linkwithlibraries)

all-local: Joystick JoystickService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
