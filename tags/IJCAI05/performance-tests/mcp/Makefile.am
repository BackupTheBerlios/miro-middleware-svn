## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/b21Base/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
