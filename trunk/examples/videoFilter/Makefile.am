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


bin_PROGRAMS = GrayVideoService


GrayVideoService_SOURCES = \
	GrayVideoService.cpp \
	FilterGray.cpp \
	FilterGray.h \
	FilterDiff.cpp \
	FilterDiff.h \
	FilterBusy.cpp \
	FilterBusy.h \
	Parameters.xml

Parameters.h: Parameters.cpp

if COND_IEEE1394
1394libraries = \
        -lraw1394 \
        -ldc1394_control
endif 

if COND_PLAYER
playerlibraries = \
	$(PLAYER_LDFLAGS) $(PLAYER_LIBS)
endif

GrayVideoService_LDADD = \
	-lmiroVideo \
	-lmiro \
        $(1394libraries) \
	$(playerlibraries)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: GrayVideoService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
