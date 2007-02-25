## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/can/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libcan.a


sources = \
	Parameters.xml \
	CanConnection.cpp \
	CanMessage.cpp \
        Can0Message.cpp \
        PCanMessage.cpp \
	CanEventHandler.cpp

Parameters.h: Parameters.cpp

libcan_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

extraheader = \
	canioctl.h \
	canmsg.h \
	pcan.h \
	can.h

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
can_include_HEADERS = $(includefiles2)

all-local: libcan.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
