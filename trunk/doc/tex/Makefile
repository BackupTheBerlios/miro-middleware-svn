## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## for details copyright, usage and credits to other groups see Miro/COPYRIGHT
## for documentation see Miro/doc
## 
## (c) 1999,2000
## Department of Neural Information Processing, University of Ulm, Germany
##
## Authors: 
##   Stefan Enderle, 
##   Stefan Sablatnoeg, 
##   Hans Utz
## 
## $Id$
## $Date$
## 
## Miro/doc/tex/Makefile
##
##############################################################################

# Makefile for Miro TeX documentation

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------


FIGDIR = ../fig
FIGS = \
	$(FIGDIR)/client_server.eps \
	$(FIGDIR)/client_server_low_level.eps \
	$(FIGDIR)/grid_coords.eps

MANUAL_FILES = \
	miro_manual.tex \
	definitions.tex \
	using.tex \
	servers.tex \
	clients.tex \
	writing_a_client.tex \
	writing_a_server.tex

REFERENCE_FILES = \
	miro_reference.tex \
	styleguide.tex

EXAMPLE_LINKS = \
	$(MIRO_ROOT)/examples/sonar/SonarPoll1.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarPoll2.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarPoll3.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarNotify.cpp \
	$(MIRO_ROOT)/examples/b21Behaviours/LaserAvoid.cpp

DOXYCONFTMPL = doxygen.conf.tmpl
DOXYCONF = doxygen.conf

# insert here the subdirs to process with doxygen
IDL_DIRS = \
	../../idl
HEADER_DIRS = \
	../../src/miro
#	../../src/abus \
#	../../src/laser \
#	../../src/mcp \
#	../../src/msp \
#	../../src/base \
#	../../src/b21Base \
#	../../src/buttons \
#	../../src/can \
#	../../src/image \
#	../../src/panTilt \
#	../../src/pioneer \
#	../../src/pioneerBase \
#	../../src/psos \
#	../../src/sparrow \
#	../../src/sparrowBase \
#	../../src/speech \
#	../../src/video

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
DEPS := $(addprefix $(VDIR),$(DEPS))
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

all:  links manual reference

links:
	-ln -s $(EXAMPLE_LINKS) .

manual: miro_manual.ps

reference: miro_reference.ps

clean:
	$(MIRO_ROOT)/scripts/doclinks_delete.sh
	-rm -f *.aux *.log *.ind *.idx *.ilg *.toc *.dvi *.bbl *.blg
	-rm -f latex_ref/*.tex latex_ref/*.eps latex_ref/*.log latex_ref/doxygen.sty latex_ref/Makefile
	-rmdir latex_ref
	-rm -f $(DOXYCONF)
	-rm -f $(FIGS)

miro_manual.dvi: $(MANUAL_FILES) $(FIGS)
	latex miro_manual.tex
	bibtex miro_manual
	latex miro_manual.tex
	makeindex miro_manual
	latex miro_manual.tex

miro_reference.dvi: latex_reference $(REFERENCE_FILES)
	$(MIRO_ROOT)/scripts/doclinks_create.sh
	latex miro_reference.tex
	latex miro_reference.tex
	makeindex miro_reference
	latex miro_reference.tex

latex_reference: $(addsuffix /*.idl,$(basename $(IDL_DIRS))) $(addsuffix /*.h,$(basename $(HEADER_DIRS)))
	cp $(DOXYCONFTMPL) $(DOXYCONF)
	@echo "# ------------------------------------" >> $(DOXYCONF)
	@echo "# automatically added input directorys" >> $(DOXYCONF)
	@echo "#-------------------------------------" >> $(DOXYCONF)
	@echo "INPUT = $(IDL_DIRS) $(HEADER_DIRS)" >> $(DOXYCONF)
	doxygen doxygen.conf
