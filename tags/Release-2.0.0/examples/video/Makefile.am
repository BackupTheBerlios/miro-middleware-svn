## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/video/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	LogVideo \
	VideoLatency \
	VideoExample


LogVideo_SOURCES = LogVideo.cpp

VideoLatency_SOURCES = VideoLatency.cpp

VideoExample_SOURCES = VideoExample.cpp

libraries = -lmiro

LogVideo_LDADD = $(libraries)
VideoLatency_LDADD = $(libraries)
VideoExample_LDADD = $(libraries)

all-local: LogVideo VideoLatency VideoExample
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
