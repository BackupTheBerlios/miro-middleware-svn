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


bin_PROGRAMS = GrayVideoService


GrayVideoService_SOURCES = \
	GrayVideoService.cpp \
	FilterGray.cpp \
	FilterGray.h \
	FilterDiff.cpp \
	FilterDiff.h \
	Parameters.xml

if COND_IEEE1394
1394libraries = \
        -lraw1394 \
        -ldc1394_control
endif 

GrayVideoService_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroVideo \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS) \
        $(1394libraries)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
