## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/battery/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = BatteryGet


BatteryGet_SOURCES = BatteryGet.cpp

BatteryGet_LDADD = \
	-lmiro 

all-local: BatteryGet
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
