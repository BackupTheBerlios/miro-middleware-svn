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
	-lsparrow \
	-lcan \
	-lpioneer \
	-lpsos \
	-lbase \
	-lmsp \
	-labus \
	-lmcp \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS) \
	$(top_builddir)/src/laser/Parameters.o \
	$(top_builddir)/src/video/Parameters.o \
	../notify/Parameters.o

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
