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
## Miro/src/b21Base/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = B21Base B21BaseDummy


B21Base_SOURCES = B21Base.cpp B21Base.h

B21BaseDummy_SOURCES = B21BaseDummy.cpp B21BaseDummy.h

linkwithlibraries = \
	-lB21Buttons \
	-lmsp \
	-labus \
	-lbase \
	-lmcp \
	-lmiroSvc \
        -lmiroIDL \
	-lmiro

B21Base_LDADD = $(linkwithlibraries)
B21BaseDummy_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
