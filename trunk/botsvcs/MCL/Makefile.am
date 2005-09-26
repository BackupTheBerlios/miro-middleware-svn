## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2005
## Department of Neural Information Processing, University of Ulm, Germany
##
## Miro/template/Makefile.am.lib.so
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_shared.h
endif


## shared library to be built
lib_LTLIBRARIES = libmiroMCL.la

## sourcefiles needed for the library to be built
sources = \
	Feature.cpp \
	FeatureManager.cpp \
	FeatureRepository.cpp \
	FeatureManagementI.cpp \
	WorldModel.cpp 

## the prefix of the following macro must have the same name as the respective library
## (replace dots with underscores)
libmiroMCL_la_SOURCES = $(sources)

## if there are headers without a associated cpp-file
extraheader = 	\
		ParticleFilter.h \
		Sample.h \
		ObservationModelT.h

## automatically builded files (depends on your source files)
BUILT_SOURCES = 
CLEANFILES = $(BUILT_SOURCES)

includefiles1 = $(sources:.xml=.h) $(extraheader)
includefiles2 = $(includefiles1:.cpp=.h)
## the prefix of the following macro indicates the installation directory
miro_include_HEADERS = $(includefiles2)

all-local: libmiroMCL.la
	$(INSTALLLTLIBRARIES)

clean-local:
	$(CLEANLTLIBRARIES)