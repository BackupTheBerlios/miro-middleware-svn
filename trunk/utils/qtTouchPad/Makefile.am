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
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = QtTouchPad


sources = \
	QtTouchPad.cpp \
	MainForm.h

tomocsources = \
	MainForm.cpp

QtTouchPad_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

QtTouchPad_LDADD = \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: QtTouchPad
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
