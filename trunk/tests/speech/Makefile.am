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
## Miro/tests/speech/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SpeechTest


SpeechTest_SOURCES = SpeechTest.cpp

SpeechTest_LDADD = \
	-lmiro

all-local: SpeechTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
