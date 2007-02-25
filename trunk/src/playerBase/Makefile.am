## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/playerBase/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
AM_CXXFLAGS += $(PLAYER_CPPFLAGS)
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = PlayerBase


PlayerBase_SOURCES = \
	Parameters.xml \
	PlayerBase.cpp \
	PlayerReactorTask.cpp \
	PlayerLaserImpl.cpp \
	PlayerMotionImpl.cpp \
	PlayerPanTiltImpl.cpp \
	PlayerCameraControlImpl.cpp \
	PlayerStallImpl.cpp

Parameters.h: Parameters.cpp

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
