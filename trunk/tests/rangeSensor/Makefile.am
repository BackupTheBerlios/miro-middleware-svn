## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/tests/rangeSensor/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = RangeSensorTest


RangeSensorTest_SOURCES = RangeSensorTest.cpp

RangeSensorTest_LDADD = \
	-lmiro

all-local: RangeSensorTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
