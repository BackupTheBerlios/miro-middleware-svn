## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/panTilt/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = DpPanTilt


DpPanTilt_SOURCES = \
	Parameters.xml \
	DpPanTilt.cpp \
	DpPanTilt.h \
	PanTiltConnection.cpp \
	PanTiltConnection.h \
	PanTiltConsumer.cpp \
	PanTiltConsumer.h \
	PanTiltEventHandler.cpp \
	PanTiltEventHandler.h \
	PanTiltData.cpp \
	PanTiltData.h \
	PanTiltImpl.cpp \
	PanTiltImpl.h \
	PanTiltMessage.h \
	PanTiltOpcodes.h

Parameters.h: Parameters.cpp

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

DpPanTilt_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local: DpPanTilt
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
