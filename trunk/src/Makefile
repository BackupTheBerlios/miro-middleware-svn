## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/src/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#       Include macros
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

##
## list all the sub directories in this variable
##

DIRS =		miro \
		video
ifneq (,$(findstring B21,$(MIRO_COMPONENTS)))
  DIRS += 	abus \
		mcp \
		msp \
		base \
		buttons \
		b21Base \
		panTilt \
		speech \
		laser
endif
## 
## Compile the Sphinx/Festival Speech server only if Sphinx and Festival are installed
##
ifdef SPHINX_ROOT
ifdef FESTIVAL_ROOT
ifdef SPEECH_TOOLS_ROOT
  DIRS += sphinx
endif
endif
endif

ifneq (,$(findstring Pioneer1,$(MIRO_COMPONENTS)))
  DIRS +=	psos \
		pioneer \
		pioneerBase
endif
ifneq (,$(findstring Sparrow99,$(MIRO_COMPONENTS)))
  ifeq (,$(findstring Pioneer1, $(MIRO_COMPONENTS)))
    DIRS +=	psos \
		pioneer
  endif
  DIRS += \
		can \
		faulTty \
		faulMotor \
		sparrow \
		sparrowBase
endif
ifneq (,$(findstring Deprecated,$(MIRO_COMPONENTS)))
  DIRS +=	image
endif


#----------------------------------------------------------------------------
#       Include targets
#----------------------------------------------------------------------------

include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU
