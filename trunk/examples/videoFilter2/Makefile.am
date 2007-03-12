## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/videoFilter2/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

SUFFIXES = .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlC.cpp:
	LD_LIBRARY_PATH=$(ACE_ROOT)/lib:$(ACE_ROOT)/ace $(TAO_IDL) -in -ci C.i.h -si S.i.h -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl $<

C.cppC.h: ;
C.hC.i.h: ;
C.i.hS.cpp: ;
S.cppS.h: ;
S.hS.i.h: ;

noinst_PROGRAMS = VideoService

ParametersFilter.h: ParametersFilter.cpp
ParametersFilter.cpp: ParametersFilter.xml

ParametersImage.h: ParametersImage.cpp
ParametersImage.cpp: ParametersImage.xml

VideoService_SOURCES = \
	VideoService.cpp \
	BallDetection.cpp \
	BallDetectionEventChannel.cpp \
	BallResultC.cpp \
	ParametersFilter.xml \
	ParametersImage.xml

VideoService_LDADD = \
	-lmiroVideo \
	-lmiro \
	$(1394libraries) \
	$(unicaplibraries) \
	$(playerlibraries)

BUILT_SOURCES = \
	ParametersFilter.cpp ParametersFilter.h \
	ParametersImage.cpp ParametersImage.h \
	BallResultC.cpp BallResultC.i.h BallResultC.h \
	BallResultS.cpp BallResultS.i.h BallResultS.h

CLEANFILES = $(BUILT_SOURCES) \
	BallResultS_T.cpp BallResultS_T.h BallResultS_T.inl

all-local: VideoService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
