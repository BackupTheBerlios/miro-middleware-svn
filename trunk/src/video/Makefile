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
## Miro/src/video/Makefile
##
##############################################################################

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#----------------------------------------------------------------------------
#       Define local macros
#---------------------------------------------------------------------------

# The order in this list is important as some libraries depend on
# others, please keep that in mind when adding or changing libraries.
MKLIST = Makefile.lib Makefile.bin

#----------------------------------------------------------------------------
#       Local rules
#----------------------------------------------------------------------------

all default:
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST

realclean::
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST

debug profile optimize inStall deinStall clean clobber depend rcs_info idl_stubs:
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST
