## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/examples/stall/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = StallPoll


StallPoll_SOURCES = StallPoll.cpp

StallPoll_LDADD = \
	-lmiro

all-local:
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)
