## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/templates/Makefile.am.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUFFIXES = .idl C.cpp C.h C.i.h S.cpp S.h S.i.h

.idlC.cpp:
	$(TAO_IDL) -in -Gt -Ge 0 -hc C.h -cs C.cpp -ci C.i.h -hs S.h -hT S_T.h -ss S.cpp -sT S_T.cpp -si S.i.h -st S_T.i.h -Sc -Wp,-I$(TAO_ROOT) -Wp,-I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $<

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

LogPerformance_DEPENDENCIES = $(sources)

BUILT_SOURCES = $(builtsources)
CLEANFILES = $(builtsources)

## sourcefiles needed for the different binaries to be built
## the prefix of the following macros must have the same name as the respective binary
LogPerformance_SOURCES  = $(builtsources) LogPerformance.cpp

## you may add libraries to be linked here
LogPerformance_LDADD  = -lmiroSvc -lmiro 
all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
