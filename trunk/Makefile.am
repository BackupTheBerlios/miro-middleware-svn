## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUBDIRS = \
	. \
	idl \
	src \
	tests \
	examples \
	performance-tests \
	utils \
	dtd \
	etc \
	scripts \
	bin \
	lib \
	doc

all-local:
	if test -L idl/idl; then rm idl/idl; fi
	$(LN_S) ./ idl/idl

distclean-local:
	rm -f idl/idl
	rm -f config/config.guess
	rm -f config/config.sub
	rm -f config/depcomp
	rm -f config/install-sh
	rm -f config/ltmain.sh
	rm -f config/missing
	rm -f config/mkinstalldirs

# configh_configdir = $(libdir)/Miro-`echo $(VERSION) | sed 's/\./:/g'`/include
configh_configdir = $(libdir)/Miro/include
configh_config_DATA = config.h

EXTRA_DIST = config/Make-rules

DISTCHECK_CONFIGURE_FLAGS = --enable-B21 --enable-Pioneer --enable-Sparrow99 --enable-BTTV --enable-IEEE1394 --enable-Meteor
