## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## $Id$
##
## Miro/pch/Makefile.am
##
##############################################################################


include $(top_srcdir)/config/Make-rules

.cc.lo:
	cp -f all.h all_static.h
	$(CXXCOMPILE) all_static.h
	echo "#error : PCH is not used"  > all_static.h
	cp -f all.h all_shared.h
	$(CXXCOMPILE) -fPIC -DPIC all_shared.h
	echo "#error : PCH is not used" > all_shared.h
	if $(LTCXXCOMPILE) -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" -c -o $@ $<; \
	then mv -f "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Plo"; else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; fi

noinst_LTLIBRARIES = libfoo.la

libfoo_la_SOURCES = foo.cc 

CLEANFILES = all_static.h all_static.h.gch all_shared.h all_shared.h.gch all.h.tmp1 all.h.tmp2
DISTCLEANFILES = all.h
