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
## Miro/src/speech/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules
if COND_PCH
CPPFLAGS+=-include $(top_builddir)/pch/all_static.h
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
