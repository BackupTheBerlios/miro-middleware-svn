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
## Miro/examples/speechSimple/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SpeechSimple


SpeechSimple_SOURCES = SpeechSimple.cpp

SpeechSimple_LDADD = \
	-lmiro

all-local: SpeechSimple
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
