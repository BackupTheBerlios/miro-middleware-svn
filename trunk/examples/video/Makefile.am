## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/video/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	LogVideo \
	VideoLatency \
	VideoExample


LogVideo_SOURCES = LogVideo.cpp

VideoLatency_SOURCES = VideoLatency.cpp

VideoExample_SOURCES = VideoExample.cpp

linkwithlibraries = \
	-lTAO \
	-lTAO_CosNaming \
	-lmiroVideo \
	-lmiroIDL \
	-lmiro \
	$(top_builddir)/lib/libmiroSvc.a \
	$(QT_LIBS)

if COND_IEEE1394
1394libraries = \
	-lraw1394 \
	-ldc1394_control
endif 

libraries = \
	$(linkwithlibraries) \
	$(1394libraries)

LogVideo_LDADD = $(libraries)
VideoLatency_LDADD = $(libraries)
VideoExample_LDADD = $(libraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
