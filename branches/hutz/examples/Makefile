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
##
## Miro/examples/Makefile
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

DIRS =		odometry \
		motion \
		rangeSensor \
		buttons \
		sonar \
		stall \
		video \
		notify \
		infrared \
		behaviours
ifneq (,$(findstring B21,$(MIRO_COMPONENTS)))
  DIRS +=	robotikI \
		b21Motion \
		b21Behaviours
endif
ifneq (,$(findstring Pioneer1,$(MIRO_COMPONENTS)))
  DIRS +=
endif
ifneq (,$(findstring Sparrow99,$(MIRO_COMPONENTS)))
  DIRS +=
endif
ifneq (,$(findstring Deprecated,$(MIRO_COMPONENTS)))
  DIRS +=
endif


# robotikI needs to be seperated for robot types

#----------------------------------------------------------------------------
#       Include targets
#----------------------------------------------------------------------------

include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU
