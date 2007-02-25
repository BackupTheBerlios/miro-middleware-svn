## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/examples/tcm2/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules


bin_PROGRAMS = TCM2Get TCM2Poll TCM2Stream


TCM2Get_SOURCES = TCM2Get.cpp

TCM2Get_LDADD = -lmiro 


TCM2Poll_SOURCES = TCM2Poll.cpp

TCM2Poll_LDADD = -lmiro 


TCM2Stream_SOURCES = TCM2Stream.cpp

TCM2Stream_LDADD = -lmiro 


all-local: TCM2Get TCM2Poll TCM2Stream
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

