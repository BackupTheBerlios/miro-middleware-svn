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
## Miro/utils/Makefile
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

DIRS =		\
		nsIOR \
		notify \
		logPlayer \
		rangeSensor \
		qtTouchPad \
		qtJoystick \
		policyEditor
ifneq (,$(findstring Sparrow99,$(MIRO_COMPONENTS)))
  DIRS +=	sparrowBoard
endif

#currently under develpment
OTHERS = 	makeParams

#----------------------------------------------------------------------------
#       Include targets
#----------------------------------------------------------------------------

include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU
