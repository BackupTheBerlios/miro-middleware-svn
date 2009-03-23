## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules

if COND_PCH
pchdir = pch
endif

SUBDIRS = \
	$(pchdir) \
	idl \
	src \
	tests \
	examples \
	performance-tests \
	botsvcs \
	utils \
	dtd \
	etc \
	scripts \
	bin \
	lib \
	doc

DISTCLEANFILES = \
	all.h.in \
	config/config.guess \
	config/config.sub \
	config/depcomp \
	config/install-sh \
	config/ltmain.sh \
	config/missing \
	config/mkinstalldirs

configh_configdir = $(libdir)/Miro/include
configh_config_DATA = config.h

EXTRA_DIST = config/Make-rules

DISTCHECK_CONFIGURE_FLAGS = --enable-B21 --enable-Pioneer --enable-Sparrow99 --enable-BTTV --enable-IEEE1394 --enable-Meteor --enable-QuickCam
