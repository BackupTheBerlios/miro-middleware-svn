## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/params/Makefile
##
##############################################################################

# a makefile example
# for a directory, wich has one library to be built

## this is the name of the resulting library
NAME     = miroParams

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = Class \
	Parameter \
	Parser \
	Generator \
	TextErrorHandler

DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

## you can toggle the debug and optimize flags at this point
# debug = 1
optimize = 0
# Only build static library 
# static_libs_only = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Class.o .obj/Class.so .shobj/Class.o .shobj/Class.so: Class.cpp Class.h Parameter.h

.obj/Parameter.o .obj/Parameter.so .shobj/Parameter.o .shobj/Parameter.so: Parameter.cpp Parameter.h

.obj/Parser.o .obj/Parser.so .shobj/Parser.o .shobj/Parser.so: Parser.cpp Parser.h Class.h Parameter.h Generator.h

.obj/Generator.o .obj/Generator.so .shobj/Generator.o .shobj/Generator.so: Generator.cpp Generator.h Class.h Parameter.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
