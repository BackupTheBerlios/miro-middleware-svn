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

.tag.tmp:
	test -e `echo $< | sed 's/\.*\\/.*\\///g'` || $(LN_S) $<

CLEANFILES = \
	idl/*.html idl/*.png idl/*.gif idl/*.css idl/formula.repository idl/installdox idl/MiroIDL.tag \
	cpp/*.html cpp/*.png cpp/*.gif cpp/*.css cpp/formula.repository cpp/installdox cpp/MiroCpp.tag \
	doxygen.idl doxygen.cpp

#clean-local: 
#	test -d idl && rmdir idl
#	test -d cpp && rmdir cpp

tagfiles= \
	ACE.tag \
	TAO.tag

all-local: $(tagfiles:.tag=.tmp)
	doxygen doxygen.idl
	(cd idl && ./installdox -lACE.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/ace/ -lTAO.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/tao/)
	doxygen doxygen.cpp
	(cd cpp && ./installdox -lMiroIDL.tag@../idl/ -lACE.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/ace/ -lTAO.tag@http://www.dre.vanderbilt.edu/Doxygen/Current/html/tao/)

install-data-local:
	mkdir -p $(datadir)/html/idl $(datadir)/html/cpp
	cp idl/* $(datadir)/html/idl
	cp cpp/* $(datadir)/html/cpp

clean-local:
	if test x$(abs_builddir) != x$(abs_srcdir); then rm $(tagfiles); fi

EXTRA_DIST = \
	ACE.tag \
	TAO.tag \
	doxygen.cpp \
	doxygen.idl
