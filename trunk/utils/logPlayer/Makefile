## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2002
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
##
## Miro/utils/logPlayer/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## dynamic libraries if needed
LDLIBS += -lmiroWidgets -lmiro

## program to be built
BIN 	= LogPlayer

## additional files
FILES	= \
	LogPlayer \
	ChannelManager
QT_SRC  = \
	LogFile \
	FileSet \
	MainForm

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


.obj/LogPlayer.o .obj/LogPlayer.so .shobj/LogPlayer.o .shobj/LogPlayer.so: LogPlayer.cpp ChannelManager.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h FileSet.h \
 LogFile.h MainForm.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i

.obj/ChannelManager.o .obj/ChannelManager.so .shobj/ChannelManager.o .shobj/ChannelManager.so: ChannelManager.cpp ChannelManager.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/LogFile.o .obj/LogFile.so .shobj/LogFile.o .shobj/LogFile.so: LogFile.cpp LogFile.h ChannelManager.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/TimeHelper.h

.obj/FileSet.o .obj/FileSet.so .shobj/FileSet.o .shobj/FileSet.so: FileSet.cpp FileSet.h LogFile.h ChannelManager.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h

.obj/MainForm.o .obj/MainForm.so .shobj/MainForm.o .shobj/MainForm.so: MainForm.cpp MainForm.h LogFile.h FileSet.h \
 ../widgets/FileListDialog.h \
 $(MIRO_ROOT)/src/miro/TimeHelper.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/Exception.h

.obj/LogFile.moc.o .obj/LogFile.moc.so .shobj/LogFile.moc.o .shobj/LogFile.moc.so: LogFile.moc.cpp LogFile.h

.obj/FileSet.moc.o .obj/FileSet.moc.so .shobj/FileSet.moc.o .shobj/FileSet.moc.so: FileSet.moc.cpp FileSet.h LogFile.h

.obj/MainForm.moc.o .obj/MainForm.moc.so .shobj/MainForm.moc.o .shobj/MainForm.moc.so: MainForm.moc.cpp MainForm.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
