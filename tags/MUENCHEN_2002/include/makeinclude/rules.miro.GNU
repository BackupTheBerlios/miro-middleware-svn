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
## adopted from ACE
##
##############################################################################


$(foreach ext, $(IDL_EXT_MINUS), %$(ext)): %C.h
	@

%C.h: $(MIRO_IDL_DIR)/%.idl $(TAO_IDL)
	$(TAO_IDL) $(MIRO_IDLFLAGS) $<

$(foreach ext, $(IDL_EXT2_MINUS), %$(ext)): %Cli.h
	@

%Cli.h: $(MIRO_IDL_DIR)/%.idl $(TAO_IDL)
	$(TAO_IDL) $(MIRO_IDLFLAGS) $<

%.moc.cpp: %.h
	$(MOC) $< -o $@

%.eps: %.fig
	fig2dev -L eps $< $@

%.png: %.fig
	fig2dev -L png $< $@

%.png: %.ps
	convert $< $@

%.ps: %.dvi
	dvips $< -o $@

.PHONY: distclean

distclean:
	rm -f `find . -name "lib*.a"`
	rm -f `find . -name "lib*.so"`
	rm -rf `find . -name ".obj"`
	rm -rf `find . -name ".shobj"`
	rm -f `find . -name *.moc.cpp`
	rm -f `find . -name *C.h`
	rm -f `find . -name *C.i.h`
	rm -f `find . -name *C.cpp`
	rm -f `find . -name *S.h`
	rm -f `find . -name *S.i.h`
	rm -f `find . -name *S.cpp`
	rm -f `find . -name *~`
	rm -f `find . -type l`
