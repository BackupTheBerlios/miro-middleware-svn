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
## Miro/examples/buttons/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	ButtonsPoll \
	ButtonsNotify


ButtonsPoll_SOURCES = ButtonsPoll.cpp
ButtonsNotify_SOURCES = ButtonsNotify.cpp ButtonsNotify.h

linkwithlibraries = \
	-lTAO \
	-lTAO_CosNotification \
	-lTAO_CosNaming \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

ButtonsPoll_LDADD = $(linkwithlibraries)
ButtonsNotify_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
