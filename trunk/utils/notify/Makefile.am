## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2001, 2002, 2003, 2004
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/utils/notify/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	LogNotify \
	LogTruncate \
	NotifyLogPlayer


LogNotify_SOURCES = \
	LogNotify.cpp

LogTruncate_SOURCES = \
	LogTruncate.cpp

NotifyLogPlayer_SOURCES = \
	NotifyLogPlayer.cpp \
	NotifyLogPlayer.h

linkwithlibraries = \
	-lmiroSvc \
	-lmiro

LogNotify_LDADD = $(linkwithlibraries)
LogTruncate_LDADD = $(linkwithlibraries)
NotifyLogPlayer_LDADD = $(linkwithlibraries)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
