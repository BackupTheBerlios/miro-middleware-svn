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

.tex.tmp:
	test -e `echo $< | sed 's/\.*\\/.*\\///g'` || $(LN_S) $<

texsources = \
	behaviourEngine.tex \
	blank.tex \
	clients.tex \
	definitions.tex \
	event_channel_logging.tex \
	group_communication.tex \
	install.tex \
	logging.tex \
	miro_manual.tex \
	miro_reference.tex \
	new_project.tex \
	parameters.tex \
	servers.tex \
	styleguide.tex \
	tex_definitions.tex \
	using.tex \
	video.tex \
	writing_a_client.tex \
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
	pdflatex $(srcdir)/miro_manual.tex; \
	bibtex $(srcdir)/miro_manual; \
	pdflatex $(srcdir)/miro_manual.tex; \
	makeindex $(srcdir)/miro_manual; \
	pdflatex $(srcdir)/miro_manual.tex
install = \
	miro_manual.pdf
else
todo = \
	latex $(srcdir)/miro_manual.tex; \
	bibtex $(srcdir)/miro_manual; \
	latex $(srcdir)/miro_manual.tex; \
	makeindex $(srcdir)/miro_manual; \
	latex $(srcdir)/miro_manual.tex; \
	dvips miro_manual.dvi -o miro_manual.ps
install = \
	miro_manual.ps
endif

all-local: $(examplelinks:.cpp=.tmp) $(texsources:.tex=.tmp)
	$(todo)

clean-local:
	-rm -f *.aux *.log *.ind *.idx *.ilg *.toc *.dvi *.bbl *.blg *.out 
	-rm -f miro_manual.pdf miro_manual.ps
	if test x$(abs_builddir) != x$(abs_srcdir); then rm -f $(texsources); fi

install-data-local:
	mkdir -p $(datadir)/doc
	cp $(install) $(datadir)/doc

EXTRA_DIST = $(texsources)
