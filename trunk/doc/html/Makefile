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

# Makefile for Miro HTML documentation

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

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

# tag file that are provided for other packages
MIROCPPTAG = MiroCpp.tag
MIROIDLTAG = MiroIdl.tag
# local tag files applied or linked to (with full path)
LINKTOLOCAL = 
# tag files that are hold local (with this name) but linked to via web (with full path)
LINKTOWEB = \
	http://doc.ece.uci.edu/Doxygen/Current/html/ace/ACE.tag \
	http://doc.ece.uci.edu/Doxygen/Current/html/tao/TAO.tag

# doxygen configuration files
DOXYCONFTMPL = doxygen.conf.tmpl
DOXYCONF = doxygen.conf

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

all:  idl_reference c++_reference

clean:
	-rm -f idl/*.html idl/*.gif idl/*.css idl/inStalldox idl/$(MIROIDLTAG)
	-rmdir idl
	-rm -f $(DOXYCONF).idl
	-rm -f cpp/*.html cpp/*.gif cpp/*.css cpp/inStalldox cpp/$(MIROCPPTAG)
	-rmdir cpp
	-rm -f $(DOXYCONF).cpp

idl_reference: $(addsuffix /*.idl,$(basename $(IDL_DIRS))) 
	cp $(DOXYCONFTMPL) $(DOXYCONF).idl
	@echo "# ----------------------------------------" >> $(DOXYCONF).idl
	@echo "# automatically added output dir          " >> $(DOXYCONF).idl
	@echo "#-----------------------------------------" >> $(DOXYCONF).idl
	@echo "HTML_OUTPUT = idl                         " >> $(DOXYCONF).idl
	@echo "# ----------------------------------------" >> $(DOXYCONF).idl
	@echo "# automatically added tag files           " >> $(DOXYCONF).idl
	@echo "#-----------------------------------------" >> $(DOXYCONF).idl
	@echo "GENERATE_TAGFILE = idl/$(MIROIDLTAG)      " >> $(DOXYCONF).idl
	@echo "TAGFILES = $(LINKTOLOCAL) $(notdir $(LINKTOWEB)) " >> $(DOXYCONF).idl
	@echo "# ----------------------------------------" >> $(DOXYCONF).idl
	@echo "# automatically added input directorys    " >> $(DOXYCONF).idl
	@echo "#-----------------------------------------" >> $(DOXYCONF).idl
	@echo "INPUT = $(IDL_DIRS)                       " >> $(DOXYCONF).idl
	doxygen $(DOXYCONF).idl
	-(cd idl; inStalldox $(join $(addprefix -l, $(notdir $(LINKTOLOCAL))), $(addprefix @, $(dir $(LINKTOLOCAL)))) $(join $(addprefix -l, $(notdir $(LINKTOWEB))), $(addprefix @, $(dir $(LINKTOWEB)))))

c++_reference: $(addsuffix /*.h,$(basename $(HEADER_DIRS)))
	cp $(DOXYCONFTMPL) $(DOXYCONF).cpp
	@echo "# ----------------------------------------" >> $(DOXYCONF).cpp
	@echo "# automatically added output dir          " >> $(DOXYCONF).cpp
	@echo "#-----------------------------------------" >> $(DOXYCONF).cpp
	@echo "HTML_OUTPUT = cpp                         " >> $(DOXYCONF).cpp
	@echo "# ----------------------------------------" >> $(DOXYCONF).cpp
	@echo "# automatically added tag files           " >> $(DOXYCONF).cpp
	@echo "#-----------------------------------------" >> $(DOXYCONF).cpp
	@echo "GENERATE_TAGFILE = cpp/$(MIROCPPTAG)      " >> $(DOXYCONF).cpp
	@echo "TAGFILES = idl/$(MIROIDLTAG) $(LINKTOLOCAL) $(notdir $(LINKTOWEB)) " >> $(DOXYCONF).cpp
	@echo "# ----------------------------------------" >> $(DOXYCONF).cpp
	@echo "# automatically added input directorys    " >> $(DOXYCONF).cpp
	@echo "#-----------------------------------------" >> $(DOXYCONF).cpp
	@echo "INPUT = $(HEADER_DIRS)                    " >> $(DOXYCONF).cpp
	doxygen $(DOXYCONF).cpp
	-(cd cpp; inStalldox -l $(MIROIDLTAG)@../idl/ $(join $(addprefix -l, $(notdir $(LINKTOLOCAL))), $(addprefix @, $(dir $(LINKTOLOCAL)))) $(join $(addprefix -l, $(notdir $(LINKTOWEB))), $(addprefix @, $(dir $(LINKTOWEB)))))
