## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/performance-tests/nmc/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


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

bin_PROGRAMS = NotifyPerformance NMCConsumer

NotifyPerformance_DEPENDENCIES = $(sources) $(builtsources)

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources) \
	$(sources:.idl=S_T.cpp) \
	$(sources:.idl=S_T.h) \
	$(sources:.idl=S_T.inl)

NotifyPerformance_SOURCES  = $(builtsources) NotifyPerformance.cpp PayloadConsumer.cpp
NMCConsumer_SOURCES  = $(builtsources) NMCConsumer.cpp PayloadConsumer.cpp

NotifyPerformance_LDADD  = -lmiroSvc -lmiro 
NMCConsumer_LDADD  = -lmiroSvc -lmiro 

all-local: NotifyPerformance NMCConsumer
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
