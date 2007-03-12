## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/videoFilter/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


noinst_PROGRAMS = GrayVideoService


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

GrayVideoService_LDADD = \
	-lmiroVideo \
	-lmiro \
	$(1394libraries) \
	$(unicaplibraries) \ 
	$(playerlibraries)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local: GrayVideoService
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
