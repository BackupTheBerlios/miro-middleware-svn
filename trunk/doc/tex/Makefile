## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## for details copyright, usage and credits to other groups see Miro/COPYRIGHT
## for documentation see Miro/doc
## 
## (c) 1999, 2000, 2001, 2002, 2003
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

# Variables for the miro manual

# directory where the figures reside
FIGDIR = ../fig
# list of figures (without extension)
FIGS = \
	$(FIGDIR)/client_server \
	$(FIGDIR)/client_server_low_level \
	$(FIGDIR)/grid_coords \
	$(FIGDIR)/Miro_behaviours

# tex-files the manual is build from
MANUAL_FILES = \
	miro_manual.tex \
	definitions.tex \
	using.tex \
	servers.tex \
	clients.tex \
	writing_a_client.tex \
	behaviourEngine.tex \
	writing_a_server.tex

# examples used in the manual
EXAMPLE_LINKS = \
	$(MIRO_ROOT)/examples/sonar/SonarPoll1.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarPoll2.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarPoll3.cpp \
	$(MIRO_ROOT)/examples/sonar/SonarNotify.cpp \
	$(MIRO_ROOT)/examples/b21Behaviours/LaserAvoid.cpp \
	$(MIRO_ROOT)/examples/video/VideoExample.cpp

# generate the list of figures for the different output-formats
EPSFIGS = $(addsuffix .eps,$(FIGS))
PNGFIGS = $(addsuffix .png,$(FIGS) $(FIGDIR)/pincolla $(FIGDIR)/signature)

# Variables for the miro reference

# tex-files the manual is build from
#REFERENCE_FILES = \
#	miro_reference.tex \
#	styleguide.tex

# stuff for the automatically build tex-reference 
#DOXYCONFTMPL = doxygen.conf.tmpl
#DOXYCONF = doxygen.conf

# insert here the subdirs to process with doxygen
#IDL_DIRS = \
#	../../idl
#HEADER_DIRS = \
#	../../src/miro
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
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

all:  links manual #reference

links:
	-ln -s $(EXAMPLE_LINKS) .

manual: miro_manual.pdf miro_manual.ps # don't change this sequence

#reference: miro_reference.ps

clean:
	$(MIRO_ROOT)/scripts/doclinks_delete.sh
	-rm -f *.aux *.log *.ind *.idx *.ilg *.toc *.dvi *.bbl *.blg *.out
	-rm -f latex_ref/*.tex latex_ref/*.eps latex_ref/*.log latex_ref/doxygen.sty latex_ref/Makefile
#	-rmdir latex_ref
#	-rm -f $(DOXYCONF)
	-rm -f $(EPSFIGS) $(PNGFIGS)

miro_manual.dvi: $(MANUAL_FILES) $(EPSFIGS)
	latex miro_manual.tex
	bibtex miro_manual
	latex miro_manual.tex
	makeindex miro_manual
	latex miro_manual.tex

miro_manual.pdf: $(MANUAL_FILES) $(PNGFIGS)
	pdflatex miro_manual.tex
	bibtex miro_manual
	pdflatex miro_manual.tex
	makeindex miro_manual
	pdflatex miro_manual.tex

#miro_reference.dvi: latex_reference $(REFERENCE_FILES)
#	$(MIRO_ROOT)/scripts/doclinks_create.sh
#	latex miro_reference.tex
#	latex miro_reference.tex
#	makeindex miro_reference
#	latex miro_reference.tex

#latex_reference: $(addsuffix /*.idl,$(basename $(IDL_DIRS))) $(addsuffix /*.h,$(basename $(HEADER_DIRS)))
#	cp $(DOXYCONFTMPL) $(DOXYCONF)
#	@echo "# ------------------------------------" >> $(DOXYCONF)
#	@echo "# automatically added input directorys" >> $(DOXYCONF)
#	@echo "#-------------------------------------" >> $(DOXYCONF)
#	@echo "INPUT = $(IDL_DIRS) $(HEADER_DIRS)" >> $(DOXYCONF)
#	doxygen doxygen.conf
