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
## Miro/demo/qtSonar/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


SUFFIXES = .h .moc.cpp

.h.moc.cpp:
	$(QT_MOC) $< -o $@


bin_PROGRAMS = QtRangeSensor


sources = \
	QtRangeSensor.h

tomocsources = \
	QtRangeSensor.cpp

QtRangeSensor_SOURCES = $(sources) $(tomocsources) $(sources:.cpp=.h) $(tomocsources:.cpp=.moc.cpp)

QtRangeSensor_LDADD = \
	-lmiro \
	$(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: QtRangeSensor
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
