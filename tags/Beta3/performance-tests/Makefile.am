## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/performance-tests/Makefile
##
##############################################################################

globaldirs = \
	video \
	panTilt

if COND_B21
b21dirs = \
	mcp
endif

if COND_SPARROW99
sparrowdirs = \
	can\
	response
endif

if COND_DEPRECATED
deprecateddirs = \
	image
endif



SUBDIRS = $(globaldirs) $(b21dirs) $(sparrowdirs) $(deprecateddirs)
