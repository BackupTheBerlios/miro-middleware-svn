## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUBDIRS = \
	idl \
	src \
	tests \
	examples \
	performance-tests \
	utils \
	doc \
	dtd \
	etc \
	scripts \
	bin \
	lib


# configh_configdir = $(libdir)/Miro-`echo $(VERSION) | sed 's/\./:/g'`/include
configh_configdir = $(libdir)/Miro/include
configh_config_DATA = config.h

EXTRA_DIST = config/Make-rules

DISTCHECK_CONFIGURE_FLAGS = --enable-B21 --enable-Pioneer --enable-Sparrow99 --enable-BTTV --enable-IEEE1394 --enable-Meteor
