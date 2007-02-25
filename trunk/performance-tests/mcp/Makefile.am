## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/mcp/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = McpPerformance


McpPerformance_SOURCES = McpPerformance.cpp McpPerformance.h

McpPerformance_LDADD = \
	$(top_builddir)/lib/libbase.a \
	$(top_builddir)/lib/libmcp.a \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiro

all-local: McpPerformance
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
