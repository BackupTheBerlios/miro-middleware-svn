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
## Miro/utils/notify/Makefile.bin
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = \
	LogNotify \
	LogTruncate \
	NotifyLogPlayer


LogNotify_SOURCES = LogNotify.cpp LogNotifyConsumer.cpp Parameters.xml
LogTruncate_SOURCES = LogTruncate.cpp
NotifyLogPlayer_SOURCES = NotifyLogPlayer.cpp

linkwithlibraries = \
	-lmiroSvc \
	-lmiroIDL \
	-lmiro \
	$(QT_LIBS)

LogNotify_LDADD = $(linkwithlibraries)
LogTruncate_LDADD = $(linkwithlibraries)
NotifyLogPlayer_LDADD = $(linkwithlibraries)

BUILT_SOURCES = Parameters.cpp Parameters.h
CLEANFILES = $(BUILT_SOURCES)

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
