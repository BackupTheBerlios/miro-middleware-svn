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
## Miro/templates/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


noinst_PROGRAMS = SimpleBaseClient


SimpleBaseClient_SOURCES = SimpleBaseClient.cpp

SimpleBaseClient_LDADD = \
	-lmiro

all-local: SimpleBaseClient
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
