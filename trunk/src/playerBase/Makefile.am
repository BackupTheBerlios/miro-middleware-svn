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
## Miro/src/pioneerBase/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_CXXFLAGS += $(PLAYER_CPPFLAGS)
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PlayerBase


PlayerBase_SOURCES = \
	Parameters.xml \
	PlayerBase.cpp \
	PlayerReactorTask.cpp \
	PlayerLaserImpl.cpp \
	PlayerMotionImpl.cpp \
	PlayerPanTiltImpl.cpp \
	PlayerStallImpl.cpp

PlayerBase_LDADD = \
	-lmiroIDL \
	-lmiro \
	-lmiroSvc \
	-lmiroLaser \
	$(PLAYER_LDFLAGS) $(PLAYER_LIBS)

all-local: PlayerBase
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
