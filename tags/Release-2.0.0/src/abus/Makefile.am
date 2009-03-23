## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/abus/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libabus.a


sources = \
	AbusConnection.cpp \
	AbusDevice.cpp \
	AbusEventHandler.cpp

libabus_a_SOURCES = $(sources)


extraheader = \
	AbusMessage.h

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
abus_include_HEADERS = $(includefiles2)

all-local: libabus.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
