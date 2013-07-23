## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/battery/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = BatteryGet


BatteryGet_SOURCES = BatteryGet.cpp

BatteryGet_LDADD = \
	-lmiro 

all-local: BatteryGet
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
