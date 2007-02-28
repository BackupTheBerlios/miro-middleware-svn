## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/logging/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .cpp .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlC.cpp:
	LD_LIBRARY_PATH=$(ACE_ROOT)/lib:$(ACE_ROOT)/ace $(TAO_IDL) -in -ci C.i.h -si S.i.h -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl $<

C.cppC.h: ;

C.hC.i.h: ;

C.i.hS.cpp: ;

S.cppS.h: ;

S.hS.i.h: ;

sources = \
	Payload.idl \
	SharedBeliefState.idl

builtsources = \
	$(sources:.idl=C.cpp) \
	$(sources:.idl=C.h) \
	$(sources:.idl=C.i.h) \
	$(sources:.idl=S.h) \
	$(sources:.idl=S.cpp) \
	$(sources:.idl=S.i.h)

## binaries to be built
bin_PROGRAMS = LogPerformance

LogPerformance_DEPENDENCIES = $(sources) $(builtsources)

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources)

## sourcefiles needed for the different binaries to be built
## the prefix of the following macros must have the same name as the respective binary
LogPerformance_SOURCES  = $(builtsources) LogPerformance.cpp

## you may add libraries to be linked here
LogPerformance_LDADD  = -lmiroSvc -lmiro 

all-local: LogPerformance
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
