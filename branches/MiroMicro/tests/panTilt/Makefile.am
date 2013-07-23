## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/panTilt/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif

if COND_SPARROW99
sparrowprograms =  PanTest2003 PanStream2003
endif

bin_PROGRAMS = PanTiltTest PanTest $(sparrowprograms)


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


all-local: PanTiltTest PanTest $(sparrowprograms)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
