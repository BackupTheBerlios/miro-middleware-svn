## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/makeConfig/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = MakeConfig


MakeConfig_SOURCES = MakeConfig.cpp

MakeConfig_LDADD = \
	$(top_builddir)/lib/libsparrow.a \
	$(top_builddir)/lib/libcan.a \
	$(top_builddir)/lib/libpioneer.a \
	$(top_builddir)/lib/libpsos.a \
	$(top_builddir)/lib/libbase.a \
	$(top_builddir)/lib/libmsp.a \
	$(top_builddir)/lib/libabus.a \
	$(top_builddir)/lib/libmcp.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro \
	$(QT_LIBS) \
	../notify/Parameters.o

all-local: MakeConfig
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
