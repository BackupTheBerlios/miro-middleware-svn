## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/templates/Makefile.lib
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`

SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_BINDIR)/moc $< -o $@


lib_LTLIBRARIES = libmiroWidgets.la


normalsources = 

tomocsources = \
	FileListDialog.cpp

libmiroWidgets_la_SOURCES = \
	$(normalsources) \
	$(tomocsources) \
	$(tomocsources:.cpp=.moc.cpp) \
	$(normalsources:.cpp=.h) \
	$(tomocsources:.cpp=.h)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(tomocsources:.cpp=.moc.cpp)

all-local:
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
