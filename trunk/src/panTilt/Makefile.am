## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/panTilt/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


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
