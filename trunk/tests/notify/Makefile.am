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
## Miro/test/notify/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = NotifyTest


NotifyTest_SOURCES = \
	NotifyTest.cpp \
	NotifyTest.h

NotifyTest_LDADD = \
	-lTAO \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
