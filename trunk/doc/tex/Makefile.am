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

include $(top_srcdir)/config/Make-rules

SUFFIXES = .tmp

.cpp.tmp:
	test -e `echo $< | sed 's/\.*\\/.*\\///g'` || $(LN_S) $<

texsources = \
	miro_manual.tex \
	definitions.tex \
	using.tex \
	servers.tex \
	clients.tex \
	writing_a_client.tex \
	behaviourEngine.tex \
	writing_a_server.tex

examplelinks = \
	$(top_srcdir)/examples/sonar/SonarPoll1.cpp \
	$(top_srcdir)/examples/sonar/SonarPoll2.cpp \
	$(top_srcdir)/examples/sonar/SonarPoll3.cpp \
	$(top_srcdir)/examples/sonar/SonarNotify.cpp \
	$(top_srcdir)/botsvcs/examples/BAP/b21/LaserAvoid.cpp \
	$(top_srcdir)/examples/video/VideoExample.cpp

if COND_PDF
todo = \
	pdflatex miro_manual.tex; \
	bibtex miro_manual; \
	pdflatex miro_manual.tex; \
	makeindex miro_manual; \
	pdflatex miro_manual.tex
install = \
	miro_manual.pdf
else
todo = \
	latex miro_manual.tex; \
	bibtex miro_manual; \
	latex miro_manual.tex; \
	makeindex miro_manual; \
	latex miro_manual.tex; \
	dvips miro_manual.dvi -o miro_manual.ps
install = \
	miro_manual.ps
endif

all-local: $(examplelinks:.cpp=.tmp) $(texsources)
	$(todo)

clean-local:
	-rm -f *.aux *.log *.ind *.idx *.ilg *.toc *.dvi *.bbl *.blg *.out 
	-rm -f miro_manual.pdf miro_manual.ps

install-data-local:
	mkdir -p $(datadir)/doc
	cp $(install) $(datadir)/doc

EXTRA_DIST = $(texsources)
