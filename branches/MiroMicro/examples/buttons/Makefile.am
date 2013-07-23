## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/buttons/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = \
	ButtonsPoll \
	ButtonsNotify


ButtonsPoll_SOURCES = ButtonsPoll.cpp
ButtonsNotify_SOURCES = ButtonsNotify.cpp ButtonsNotify.h

linkwithlibraries = \
	-lmiro

ButtonsPoll_LDADD = $(linkwithlibraries)
ButtonsNotify_LDADD = $(linkwithlibraries)

all-local: ButtonsPoll ButtonsNotify
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
