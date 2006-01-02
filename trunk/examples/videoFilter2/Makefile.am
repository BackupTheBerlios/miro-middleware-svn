## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/videoFilter/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

SUFFIXES = .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlS.cpp:
	$(TAO_IDL) -in -Ge 0 -hc C.h -cs C.cpp -ci C.i.h -hs S.h -hT S_T.h -ss S.cpp -sT S_T.cpp -si S.i.h -st S_T.i.h -Sc -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl $<



bin_PROGRAMS = VideoService


ParametersFilter.h: ParametersFilter.cpp
ParametersFilter.cpp: ParametersFilter.xml

ParametersImage.h: ParametersImage.cpp
ParametersImage.cpp: ParametersImage.xml

BallResultC.cpp: BallResultC.i.h
BallResultC.i.h: BallResultC.h
BallResultC.h: BallResultS.cpp
BallResultS.i.h: BallResultS.h
BallResultS.h: BallResult.idl

VideoService_SOURCES = \
	VideoService.cpp \
	BallDetection.cpp \
	BallDetectionEventChannel.cpp \
	BallResultC.cpp \
	ParametersFilter.xml \
	ParametersImage.xml


if COND_IEEE1394
1394libraries = \
        -lraw1394 \
        -ldc1394_control
endif 

if COND_PLAYER
playerlibraries = \
	$(PLAYER_LDFLAGS) $(PLAYER_LIBS)
endif

VideoService_LDADD = \
	-lmiroVideo \
	-lmiro \
        $(1394libraries) \
	$(playerlibraries)

BUILT_SOURCES = \
	ParametersFilter.cpp ParametersFilter.h \
	ParametersImage.cpp ParametersImage.h \
	BallResultC.cpp BallResultC.i.h BallResultC.h \
	BallResultS.cpp BallResultS.i.h BallResultS.h
CLEANFILES = $(BUILT_SOURCES)

all-local: VideoService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
