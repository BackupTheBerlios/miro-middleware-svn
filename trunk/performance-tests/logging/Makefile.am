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

#	LD_LIBRARY_PATH=$(ACE_ROOT)/lib:$(ACE_ROOT)/ace $(TAO_IDL) -in -ci C.i.h -si S.i.h -Wp,-I$(top_srcdir)/idl -I$(top_srcdir)/idl -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $<

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

bin_PROGRAMS = LogPerformance

LogPerformance_DEPENDENCIES = $(sources) $(builtsources)

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources) \
	$(sources:.idl=S_T.cpp) \
	$(sources:.idl=S_T.h) \
	$(sources:.idl=S_T.inl)

LogPerformance_SOURCES  = $(builtsources) LogPerformance.cpp
LogPerformance_LDADD  = -lmiroSvc -lmiro 

all-local: LogPerformance
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
