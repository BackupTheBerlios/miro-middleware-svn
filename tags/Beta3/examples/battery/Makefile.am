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


bin_PROGRAMS = BatteryGet


BatteryGet_SOURCES = BatteryGet.cpp

BatteryGet_LDADD = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
