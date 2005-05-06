## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = GpsPoll GpsStream CtGpsDemo


GpsPoll_SOURCES = GpsPoll.cpp

GpsPoll_LDADD = -lmiro 


GpsStream_SOURCES = GpsStream.cpp GpsStream.h

GpsStream_LDADD = -lmiro


CtGpsDemo_SOURCES = CtGpsDemo.cpp

CtGpsDemo_LDADD = -lmiro

all-local: GpsPoll GpsStream CtGpsDemo
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

