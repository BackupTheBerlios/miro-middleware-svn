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
## Miro/demo/qtJoystick/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules

SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = QtJoystick


sources = \
	QtJoystick.cpp

tomocsources = \
	MainForm.cpp \
	MainForm.h

QtJoystick_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

QtJoystick_LDADD = \
	-lTAO \
	-lTAO_CosNaming \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(tomocsources:.cpp=.moc.cpp)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
