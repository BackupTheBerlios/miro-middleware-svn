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


bin_PROGRAMS = PanTiltTest PanTest PanTest2003 PanStream2003


PanTiltTest_SOURCES = PanTiltTest.cpp

PanTiltTest_LDADD = \
	-lmiro

PanTest_SOURCES = PanTest.cpp

PanTest_LDADD = \
	-lmiro
	
PanTest2003_SOURCES = PanTest2003.cpp

PanTest2003_LDADD = \
	-lmiro
	
PanStream2003_SOURCES = PanStream2003.cpp

PanStream2003_LDADD = \
	-lmiro


all-local: PanTiltTest PanTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
