## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/tests/xml/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = Xml-parse


Xml_parse_SOURCES = Xml-parse.cpp

Xml_parse_LDADD = \
	-lmiro \
	$(QT_LIBS)

all-local: Xml-parse
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
