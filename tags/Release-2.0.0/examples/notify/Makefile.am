## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/notify/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = GenericNotify


GenericNotify_SOURCES = GenericNotify.cpp GenericNotify.h

GenericNotify_LDADD = \
	-lmiro

all-local: GenericNotify
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
