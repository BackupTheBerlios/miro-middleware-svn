## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/policyEditor/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += 

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= PolicyEditor

## additional files
FILES	= \
	PolicyDocument \
	PolicyConfig

QT_SRC  = \
	PolicyView \
	PatternWidget \
	BehaviourWidget \
	ArbiterWidget \
	ParamDialog \
	PolicyEditor \
	PolicyConfigurationDialog

FILES += $(QT_SRC)

MOC_SRC	= $(addsuffix .moc.cpp, $(QT_SRC))
SRC	= $(addsuffix .cpp,$(FILES)) $(MOC_SRC)
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(MIRO_ROOT)/include/makeinclude/macros.miro.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
.PRECIOUS: $(MOC_SRC)

realclean: clean
	-$(RM) $(MOC_SRC))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/PolicyDocument.o .obj/PolicyDocument.so .shobj/PolicyDocument.o .shobj/PolicyDocument.so: PolicyDocument.cpp PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h

.obj/PolicyConfig.o .obj/PolicyConfig.so .shobj/PolicyConfig.o .shobj/PolicyConfig.so: PolicyConfig.cpp PolicyConfig.h BehaviourDescription.h \
 BehaviourParam.h PolicyConfigurationDialog.h

.obj/PolicyView.o .obj/PolicyView.so .shobj/PolicyView.o .shobj/PolicyView.so: PolicyView.cpp PolicyView.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h \
 PatternWidget.h

.obj/PatternWidget.o .obj/PatternWidget.so .shobj/PatternWidget.o .shobj/PatternWidget.so: PatternWidget.cpp PatternWidget.h PolicyView.h \
 PolicyDocument.h BehaviourDescription.h Transition.h PolicyConfig.h \
 BehaviourParam.h BehaviourWidget.h ArbiterWidget.h

.obj/BehaviourWidget.o .obj/BehaviourWidget.so .shobj/BehaviourWidget.o .shobj/BehaviourWidget.so: BehaviourWidget.cpp BehaviourWidget.h \
 PatternWidget.h PolicyView.h PolicyDocument.h BehaviourDescription.h \
 Transition.h PolicyConfig.h BehaviourParam.h ParamDialog.h

.obj/ArbiterWidget.o .obj/ArbiterWidget.so .shobj/ArbiterWidget.o .shobj/ArbiterWidget.so: ArbiterWidget.cpp ArbiterWidget.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h \
 PatternWidget.h PolicyView.h

.obj/ParamDialog.o .obj/ParamDialog.so .shobj/ParamDialog.o .shobj/ParamDialog.so: ParamDialog.cpp ParamDialog.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h

.obj/PolicyEditor.o .obj/PolicyEditor.so .shobj/PolicyEditor.o .shobj/PolicyEditor.so: PolicyEditor.cpp PolicyEditor.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h \
 PolicyView.h

.obj/PolicyConfigurationDialog.o .obj/PolicyConfigurationDialog.so .shobj/PolicyConfigurationDialog.o .shobj/PolicyConfigurationDialog.so: PolicyConfigurationDialog.cpp \
 PolicyConfigurationDialog.h BehaviourDescription.h BehaviourParam.h \
 PolicyConfig.h

.obj/PolicyView.moc.o .obj/PolicyView.moc.so .shobj/PolicyView.moc.o .shobj/PolicyView.moc.so: PolicyView.moc.cpp PolicyView.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h

.obj/PatternWidget.moc.o .obj/PatternWidget.moc.so .shobj/PatternWidget.moc.o .shobj/PatternWidget.moc.so: PatternWidget.moc.cpp PatternWidget.h \
 PolicyView.h PolicyDocument.h BehaviourDescription.h Transition.h \
 PolicyConfig.h BehaviourParam.h

.obj/BehaviourWidget.moc.o .obj/BehaviourWidget.moc.so .shobj/BehaviourWidget.moc.o .shobj/BehaviourWidget.moc.so: BehaviourWidget.moc.cpp BehaviourWidget.h \
 PatternWidget.h PolicyView.h PolicyDocument.h BehaviourDescription.h \
 Transition.h PolicyConfig.h BehaviourParam.h

.obj/ArbiterWidget.moc.o .obj/ArbiterWidget.moc.so .shobj/ArbiterWidget.moc.o .shobj/ArbiterWidget.moc.so: ArbiterWidget.moc.cpp ArbiterWidget.h \
 PolicyDocument.h BehaviourDescription.h Transition.h PolicyConfig.h \
 BehaviourParam.h PatternWidget.h PolicyView.h

.obj/ParamDialog.moc.o .obj/ParamDialog.moc.so .shobj/ParamDialog.moc.o .shobj/ParamDialog.moc.so: ParamDialog.moc.cpp ParamDialog.h PolicyDocument.h \
 BehaviourDescription.h Transition.h PolicyConfig.h BehaviourParam.h

.obj/PolicyEditor.moc.o .obj/PolicyEditor.moc.so .shobj/PolicyEditor.moc.o .shobj/PolicyEditor.moc.so: PolicyEditor.moc.cpp PolicyEditor.h \
 PolicyDocument.h BehaviourDescription.h Transition.h PolicyConfig.h \
 BehaviourParam.h PolicyView.h

.obj/PolicyConfigurationDialog.moc.o .obj/PolicyConfigurationDialog.moc.so .shobj/PolicyConfigurationDialog.moc.o .shobj/PolicyConfigurationDialog.moc.so: PolicyConfigurationDialog.moc.cpp \
 PolicyConfigurationDialog.h BehaviourDescription.h BehaviourParam.h \
 PolicyConfig.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
