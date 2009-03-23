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


bin_PROGRAMS = CompassGet  CompassStream


CompassGet_SOURCES = CompassGet.cpp

CompassGet_LDADD = -lmiro 




CompassStream_SOURCES = CompassStream.cpp

CompassStream_LDADD = -lmiro 


all-local: CompassGet  CompassStream
	$(INSTALLPROGRAMS)

clean-local:
	$(CLEANPROGRAMS)

