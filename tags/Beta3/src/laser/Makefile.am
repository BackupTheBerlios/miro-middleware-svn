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
## Miro/src/laser/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SickLaserService


SickLaserService_SOURCES = \
	Parameters.xml \
	SickLaserService.cpp \
	SickLaserService.h \
	SickLaserConnection.cpp \
	SickLaserConnection.h \
	SickLaserTask.cpp \
	SickLaserTask.h \
	SickLaserPollTask.cpp \
	SickLaserPollTask.h \
	SickLaserEvent.cpp \
	SickLaserEvent.h \
	SickLaserImpl.cpp \
	SickLaserImpl.h \
	SickLaserMessage.h \
	SickLaserStatistic.h \
	SickLaserOpcodes.h

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

SickLaserService_LDADD = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
