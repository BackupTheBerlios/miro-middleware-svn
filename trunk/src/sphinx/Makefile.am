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
## Miro/src/sphinx/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
AM_LDFLAGS += -version-info `echo $(VERSION) | sed 's/\./:/g'`
AM_CXXFLAGS += $(FESTIVAL_CPPFLAGS) $(SPEECHTOOLS_CPPFLAGS) $(SPHINX_CPPFLAGS)


bin_PROGRAMS = SphinxSpeech


SphinxSpeech_SOURCES = \
	FestivalSpeech.cpp \
	SphinxSpeechImpl.cpp \
	SphinxSpeechTask.cpp

SphinxSpeech_LDADD = \
	-lmiroSvc \
	$(FESTIVAL_LDFLAGS) $(FESTIVAL_LIBS) \
	$(SPEECHTOOLS_LDFLAGS) $(SPEECHTOOLS_LIBS) \
	$(SPHINX_LDFLAGS) $(SPHINX_LIBS) \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
