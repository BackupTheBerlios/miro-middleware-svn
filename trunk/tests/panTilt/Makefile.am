## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/dtlk/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = PanTiltTest PanTest


PanTiltTest_SOURCES = PanTiltTest.cpp

PanTiltTest_LDADD = \
	-lmiro

PanTest_SOURCES = PanTest.cpp

PanTest_LDADD = \
	-lmiro

all-local: PanTiltTest PanTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
