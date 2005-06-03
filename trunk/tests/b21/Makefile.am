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
## Miro/test/b21/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	B21BaseTest \
	B21StressBase \
	B21ButtonsTest \
	B21SelfTest


B21BaseTest_SOURCES = B21BaseTest.cpp
B21StressBase_SOURCES = B21StressBase.cpp
B21ButtonsTest_SOURCES = B21ButtonsTest.cpp
B21SelfTest_SOURCES = B21SelfTest.cpp

linkwithlibraries = \
	$(top_builddir)/lib/libmiroSvc.a \
	$(top_builddir)/lib/libmiroSsh.a \
	-lmiro

B21BaseTest_LDADD = $(linkwithlibraries)
B21StressBase_LDADD = $(linkwithlibraries)
B21ButtonsTest_LDADD = $(linkwithlibraries)
B21SelfTest_LDADD = $(linkwithlibraries)

all-local: B21BaseTest B21StressBase B21ButtonsTest B21SelfTest
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
