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
	PanTiltConnection.cpp \
	PanTiltConsumer.cpp \
	PanTiltEventHandler.cpp \
	PanTiltData.cpp \
	PanTiltImpl.cpp 

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

DpPanTilt_LDADD = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
