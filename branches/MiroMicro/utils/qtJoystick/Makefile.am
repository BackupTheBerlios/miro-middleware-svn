## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/utils/qtJoystick/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = QtJoystick


sources = \
	QtJoystick.cpp \
	MainForm.h

tomocsources = \
	MainForm.cpp 

QtJoystick_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

QtJoystick_LDADD = \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: QtJoystick
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
