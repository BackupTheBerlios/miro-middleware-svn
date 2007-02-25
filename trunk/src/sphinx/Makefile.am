## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/sphinx/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
AM_CXXFLAGS += $(FESTIVAL_CPPFLAGS) $(SPEECHTOOLS_CPPFLAGS) $(SPHINX_CPPFLAGS)
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = SphinxSpeech


SphinxSpeech_SOURCES = \
	FestivalSpeech.cpp \
	SphinxSpeech.cpp \
	SphinxSpeechImpl.cpp \
	SphinxSpeechTask.cpp

SphinxSpeech_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	$(FESTIVAL_LDFLAGS) $(FESTIVAL_LIBS) \
	$(SPEECHTOOLS_LDFLAGS) $(SPEECHTOOLS_LIBS) \
	$(SPHINX_LDFLAGS) $(SPHINX_LIBS) \
	-lmiro -lmiroIDL 

all-local: SphinxSpeech
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
