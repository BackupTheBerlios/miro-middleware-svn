## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 1999, 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
## adopted from ACE
##
##############################################################################

ifndef MIRO_COMPONENTS
  MIRO_COMPONENTS = B21 Pioneer1 Sparrow99 Deprecated
endif

ifndef MIRO_VIDEO_COMPONENTS
  MIRO_VIDEO_COMPONENTS = BTTV Meteor 1394
endif

# local macros

INSLIB = $(MIRO_ROOT)/lib
INSBIN  = $(MIRO_ROOT)/bin

INCLDIRS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -I$(MIRO_ROOT)/src -I$(QTDIR)/include

CCFLAGS += -Woverloaded-virtual -DDEBUG  -DQT_GENUINE_STR

LDFLAGS +=  -L$(INSLIB) -L$(ACE_ROOT)/ace

MIRO_IDL_DIR = $(MIRO_ROOT)/idl
MIRO_SRC_DIR = $(MIRO_ROOT)/src

MIRO_IDLFLAGS  = -in -Ge 0
MIRO_IDLFLAGS += -Sc -Wp,-I$(MIRO_IDL_DIR)

tao_dont_use_idl_make_rule = 1

DOXYGEN = doxygen

# add qt support if ACE/TAO was built without

ifeq ($(qt_reactor),)
  CPPFLAGS += $(PLATFORM_QT_CPPFLAGS)
  LIBS += $(PLATFORM_QT_LIBS)
  LDFLAGS += $(PLATFORM_QT_LDFLAGS)
endif # qt_reactor

