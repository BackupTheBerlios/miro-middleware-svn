## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/src/speech/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules
if COND_PCH
AM_CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
endif


bin_PROGRAMS = DtlkSpeech


DtlkSpeech_SOURCES = \
	DtlkSpeech.cpp \
	DtlkSpeech.h \
	SpeechConnection.cpp \
	SpeechConnection.h \
	SpeechImpl.cpp \
	SpeechImpl.h \
	SpeechDevice.h


DtlkSpeech_LDADD = \
	$(top_builddir)/lib/libmiroSvc.a \
	-lmiroIDL \
	-lmiro

all-local: DtlkSpeech
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
