## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/buttons/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libB21Buttons.a


sources = \
	ButtonsConnection.cpp \
	ButtonsEventHandler.cpp \
	ButtonsImpl.cpp

libB21Buttons_a_SOURCES = $(sources)


extraheader = \
	ButtonsDevice.h

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
buttons_include_HEADERS = $(includefiles2)

all-local: libB21Buttons.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
