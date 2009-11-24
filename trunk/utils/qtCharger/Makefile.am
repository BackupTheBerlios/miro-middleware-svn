## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id: Makefile.am jaklitsch $
##
## Miro/utils/qtCharger/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = QtCharger


sources = \
    QtCharger.cpp \
    MainForm.h

tomocsources = \
    MainForm.cpp 

QtCharger_SOURCES = $(sources) $(tomocsources) $(tomocsources:.cpp=.moc.cpp)

QtCharger_LDADD = \
    -lmiro \
    $(QT_LIBS)

BUILT_SOURCES = $(tomocsources:.cpp=.moc.cpp)
CLEANFILES = $(BUILT_SOURCES)

all-local: $(bin_PROGRAMS)
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
