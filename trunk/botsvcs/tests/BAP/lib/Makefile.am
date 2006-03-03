## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/botsvcs/tests/BAP/lib/Makefile.am
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif

## shared library to be built
lib_LTLIBRARIES = libmiroBAPtests.la

## sourcefiles needed for the library to be built
sources = \
	Parameters.xml \
	DummyArbiter.cpp \
	DummyArbiterMessage.cpp \
	DummyBehaviour.cpp \
	DummyBehaviourB.cpp \
	DummyBehaviourD.cpp \
	DummyFactory.cpp

Parameters.h: Parameters.cpp
Parameters.cpp: Parameters.xml

## the prefix of the following macro must have the same name as the respective library
## (replace dots with underscores)
libmiroBAPtests_la_SOURCES = $(sources)

## if there are headers without a associated cpp-file
extraheader = 

## automatically builded files (depends on your source files)
BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
## the prefix of the following macro indicates the installation directory
# miro_include_HEADERS = $(includefiles2)

all-local: libmiroBAPtests.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)
