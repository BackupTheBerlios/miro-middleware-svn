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
	-lTAO \
	-lTAO_CosNaming \
	-lmiroSsh \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

B21BaseTest_LDADD = $(linkwithlibraries)
B21StressBase_LDADD = $(linkwithlibraries)
B21ButtonsTest_LDADD = $(linkwithlibraries)
B21SelfTest_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
