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


bin_PROGRAMS = SphinxTest


SphinxTest_SOURCES = SphinxTest.cpp

SphinxTest_LDADD = \
	-lTAO \
	-lTAO_CosNaming \
        $(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
