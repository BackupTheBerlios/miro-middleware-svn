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


bin_PROGRAMS = DtlkSpeech


DtlkSpeech_SOURCES = \
	DtlkSpeech.cpp \
	SpeechConnection.cpp \
	SpeechImpl.cpp

DtlkSpeech_LDADD = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro

#EXTRA_DIST = \
#	DtlkSpeech.h \
#	SpeechConnection.h \
#	SpeechDevice.h \
#	SpeechImpl.h

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
