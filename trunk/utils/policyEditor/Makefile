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
LDLIBS += -lmiroWidgets -lmiroParams -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= PolicyEditor

## additional files
FILES	= \
	PolicyDocument \
	PolicyPattern \
	PolicyBehaviour \
	PolicyConfig

QT_SRC  = \
	PolicyView \
	PatternWidget \
	BehaviourWidget \
	ArbiterWidget \
	ParameterDialog \
	PolicyEditor

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
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(MIRO_ROOT)/include/makeinclude/rules.miro.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
.PRECIOUS: $(MOC_SRC)

depend: $(MOC_SRC)

realclean: clean
	-$(RM) $(MOC_SRC))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/PolicyDocument.o .obj/PolicyDocument.so .shobj/PolicyDocument.o .shobj/PolicyDocument.so: PolicyDocument.cpp PolicyDocument.h Transition.h \
 PolicyConfig.h

.obj/PolicyPattern.o .obj/PolicyPattern.so .shobj/PolicyPattern.o .shobj/PolicyPattern.so: PolicyPattern.cpp PolicyPattern.h Transition.h \
 PolicyDocument.h PolicyConfig.h

.obj/PolicyBehaviour.o .obj/PolicyBehaviour.so .shobj/PolicyBehaviour.o .shobj/PolicyBehaviour.so: PolicyBehaviour.cpp PolicyBehaviour.h \
 PolicyDocument.h Transition.h PolicyConfig.h

.obj/PolicyConfig.o .obj/PolicyConfig.so .shobj/PolicyConfig.o .shobj/PolicyConfig.so: PolicyConfig.cpp PolicyConfig.h ../params/Parser.h \
 ../params/Class.h ../params/Parameter.h ../params/Generator.h \
 ../params/TextErrorHandler.h

.obj/ConfigDialog.o .obj/ConfigDialog.so .shobj/ConfigDialog.o .shobj/ConfigDialog.so: ConfigDialog.cpp ConfigDialog.h

.obj/PolicyView.o .obj/PolicyView.so .shobj/PolicyView.o .shobj/PolicyView.so: PolicyView.cpp PolicyView.h PolicyDocument.h \
 Transition.h PolicyConfig.h PatternWidget.h

.obj/PatternWidget.o .obj/PatternWidget.so .shobj/PatternWidget.o .shobj/PatternWidget.so: PatternWidget.cpp PatternWidget.h PolicyView.h \
 PolicyDocument.h Transition.h PolicyConfig.h BehaviourWidget.h \
 ArbiterWidget.h ../params/Generator.h ../params/Class.h \
 ../params/Parameter.h

.obj/BehaviourWidget.o .obj/BehaviourWidget.so .shobj/BehaviourWidget.o .shobj/BehaviourWidget.so: BehaviourWidget.cpp BehaviourWidget.h \
 PatternWidget.h PolicyView.h PolicyDocument.h Transition.h \
 PolicyConfig.h ParameterDialog.h ../params/Class.h \
 ../params/Parameter.h

.obj/ArbiterWidget.o .obj/ArbiterWidget.so .shobj/ArbiterWidget.o .shobj/ArbiterWidget.so: ArbiterWidget.cpp ArbiterWidget.h PolicyDocument.h \
 Transition.h PolicyConfig.h PatternWidget.h PolicyView.h \
 ../params/Generator.h ../params/Class.h ../params/Parameter.h

.obj/ParameterDialog.o .obj/ParameterDialog.so .shobj/ParameterDialog.o .shobj/ParameterDialog.so: ParameterDialog.cpp ParameterDialog.h \
 ../params/Class.h ../params/Parameter.h PolicyConfig.h \
 ../params/Generator.h

.obj/PolicyEditor.o .obj/PolicyEditor.so .shobj/PolicyEditor.o .shobj/PolicyEditor.so: PolicyEditor.cpp PolicyEditor.h PolicyDocument.h \
 Transition.h PolicyConfig.h $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h PolicyView.h \
 ParameterDialog.h ../params/Class.h ../params/Parameter.h \
 ../widgets/FileListDialog.h \
 $(MIRO_ROOT)/src/miro/BehaviourEngineC.h \
 $(MIRO_ROOT)/src/miro/BehaviourEngineC.i

.obj/ConfigDialog.moc.o .obj/ConfigDialog.moc.so .shobj/ConfigDialog.moc.o .shobj/ConfigDialog.moc.so: ConfigDialog.moc.cpp ConfigDialog.h

.obj/PolicyView.moc.o .obj/PolicyView.moc.so .shobj/PolicyView.moc.o .shobj/PolicyView.moc.so: PolicyView.moc.cpp PolicyView.h PolicyDocument.h \
 Transition.h PolicyConfig.h

.obj/PatternWidget.moc.o .obj/PatternWidget.moc.so .shobj/PatternWidget.moc.o .shobj/PatternWidget.moc.so: PatternWidget.moc.cpp PatternWidget.h \
 PolicyView.h PolicyDocument.h Transition.h PolicyConfig.h

.obj/BehaviourWidget.moc.o .obj/BehaviourWidget.moc.so .shobj/BehaviourWidget.moc.o .shobj/BehaviourWidget.moc.so: BehaviourWidget.moc.cpp BehaviourWidget.h \
 PatternWidget.h PolicyView.h PolicyDocument.h Transition.h \
 PolicyConfig.h

.obj/ArbiterWidget.moc.o .obj/ArbiterWidget.moc.so .shobj/ArbiterWidget.moc.o .shobj/ArbiterWidget.moc.so: ArbiterWidget.moc.cpp ArbiterWidget.h \
 PolicyDocument.h Transition.h PolicyConfig.h PatternWidget.h \
 PolicyView.h

.obj/ParameterDialog.moc.o .obj/ParameterDialog.moc.so .shobj/ParameterDialog.moc.o .shobj/ParameterDialog.moc.so: ParameterDialog.moc.cpp ParameterDialog.h \
 ../params/Class.h ../params/Parameter.h

.obj/PolicyEditor.moc.o .obj/PolicyEditor.moc.so .shobj/PolicyEditor.moc.o .shobj/PolicyEditor.moc.so: PolicyEditor.moc.cpp PolicyEditor.h \
 PolicyDocument.h Transition.h PolicyConfig.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
