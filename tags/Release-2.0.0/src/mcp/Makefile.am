## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/mcp/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


lib_LIBRARIES = libmcp.a


sources = \
	Parameters.xml \
	McpDevice.cpp \
	McpMessage.cpp \
	McpConnection.cpp \
	McpConsumer.cpp \
	McpEventHandler.cpp

Parameters.h: Parameters.cpp

libmcp_a_SOURCES = $(sources)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h)
includefiles2 = $(includefiles1:.cpp=.h)
mcp_include_HEADERS = $(includefiles2)

all-local: libmcp.a
	$(INSTALLLIBRARIES)

clean-local:
	$(CLEANLIBRARIES)
