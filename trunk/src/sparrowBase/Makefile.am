## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/sparrowBase/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = SparrowBase


SparrowBase_SOURCES = \
	SparrowServer.cpp \
	SparrowBase.cpp

SparrowBase_LDADD = \
	-lsparrow \
	-lcan \
	-lpioneer \
	-lpsos \
	-lfaulMotor \
	-lfaulTty \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
