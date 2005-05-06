## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## 
## Miro/doc/html/Makefile
##
##############################################################################

include $(top_srcdir)/config/Make-rules

CLEANFILES = \
	idl/*.html idl/*.png idl/*.gif idl/*.css idl/installdox idl/MiroIDL.tag \
	cpp/*.html cpp/*.png cpp/*.gif cpp/*.css cpp/installdox cpp/MiroCpp.tag 

#clean-local: 
#	test -d idl && rmdir idl
#	test -d cpp && rmdir cpp

all-local:
	doxygen doxygen.idl
	(cd idl && ./installdox -lACE.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/ace/ -lTAO.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/tao/)
	doxygen doxygen.cpp
	(cd cpp && ./installdox -lMiroIDL.tag@../idl/ -lACE.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/ace/ -lTAO.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/tao/)

install-data-local:
	mkdir -p $(datadir)/html/idl $(datadir)/html/cpp
	cp idl/* $(datadir)/html/idl
	cp cpp/* $(datadir)/html/cpp

EXTRA_DIST = \
	ACE.tag \
	TAO.tag \
	doxygen.cpp \
	doxygen.idl
