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
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = DtlkSpeechTest


DtlkSpeechTest_SOURCES = DtlkSpeechTest.cpp

DtlkSpeechTest_LDADD = \
	-lmiro

all-local: DtlkSpeechTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
