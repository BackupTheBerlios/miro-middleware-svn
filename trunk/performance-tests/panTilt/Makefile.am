## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = PanLatency


PanLatency_SOURCES = PanLatency.cpp

PanLatency_LDADD = \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
