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
## Miro/src/image/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS +=  -lmiro -lTAO_CosNotification -lTAO_CosNaming -lTAO

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= MiroImage

## additional files
FILES	=	\
		Pixels \
		FrameGrabber \
		FrameGrabberMeteor \
		ImageSource \
		VideoSource \
		PatternSource \
		ImageFactoryDefinitions \
		ImageFactory \
		ImageServer \
		ImageImpl


SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

CCFLAGS	+=  -DMIRO -DDEBUG_LOG -DSTL_SAVVY -DVIDEO_METEOR

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

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Pixels.o .obj/Pixels.so .shobj/Pixels.o .shobj/Pixels.so: Pixels.cpp Pixels.h MiroImage.h pixels.ih

.obj/FrameGrabber.o .obj/FrameGrabber.so .shobj/FrameGrabber.o .shobj/FrameGrabber.so: FrameGrabber.cpp Pixels.h MiroImage.h pixels.ih \
 frameGrabber.h ImageFactoryDefinitions.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i

.obj/FrameGrabberMeteor.o .obj/FrameGrabberMeteor.so .shobj/FrameGrabberMeteor.o .shobj/FrameGrabberMeteor.so: FrameGrabberMeteor.cpp frameGrabberMeteor.h \
 ioctl_meteor.h frameGrabber.h ImageFactoryDefinitions.h MiroImage.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i Pixels.h pixels.ih

.obj/ImageSource.o .obj/ImageSource.so .shobj/ImageSource.o .shobj/ImageSource.so: ImageSource.cpp Pixels.h MiroImage.h pixels.ih \
 imageSource.h ImageFactory.h ImageFactoryDefinitions.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i

.obj/VideoSource.o .obj/VideoSource.so .shobj/VideoSource.o .shobj/VideoSource.so: VideoSource.cpp Pixels.h MiroImage.h pixels.ih \
 videoSource.h imageSource.h ImageFactory.h ImageFactoryDefinitions.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i

.obj/PatternSource.o .obj/PatternSource.so .shobj/PatternSource.o .shobj/PatternSource.so: PatternSource.cpp Pixels.h MiroImage.h pixels.ih \
 patternSource.h imageSource.h ImageFactory.h \
 ImageFactoryDefinitions.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i

.obj/ImageFactoryDefinitions.o .obj/ImageFactoryDefinitions.so .shobj/ImageFactoryDefinitions.o .shobj/ImageFactoryDefinitions.so: ImageFactoryDefinitions.cpp \
 ImageFactoryDefinitions.h MiroImage.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i

.obj/ImageFactory.o .obj/ImageFactory.so .shobj/ImageFactory.o .shobj/ImageFactory.so: ImageFactory.cpp ImageFactory.h \
 ImageFactoryDefinitions.h MiroImage.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i imageSource.h \
 videoSource.h patternSource.h Pixels.h pixels.ih

.obj/ImageServer.o .obj/ImageServer.so .shobj/ImageServer.o .shobj/ImageServer.so: ImageServer.cpp ImageServer.h \
 $(MIRO_ROOT)/src/miro/Server.h \
 $(MIRO_ROOT)/src/miro/Client.h \
 $(MIRO_ROOT)/src/miro/Exception.h \
 $(MIRO_ROOT)/src/miro/Parameters.h \
 $(MIRO_ROOT)/src/miro/ArbiterParameters.h \
 $(MIRO_ROOT)/src/miro/BehaviourParameters.h \
 $(MIRO_ROOT)/src/miro/ReactorTask.h ImageImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/ImageS.h \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsS.i \
 $(MIRO_ROOT)/src/miro/ImageC.h \
 $(MIRO_ROOT)/src/miro/ImageC.i \
 $(MIRO_ROOT)/src/miro/ImageS.i ImageFactory.h \
 ImageFactoryDefinitions.h MiroImage.h

.obj/ImageImpl.o .obj/ImageImpl.so .shobj/ImageImpl.o .shobj/ImageImpl.so: ImageImpl.cpp ImageImpl.h \
 $(MIRO_ROOT)/src/miro/OdometryC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.h \
 $(MIRO_ROOT)/src/miro/ExceptionC.i \
 $(MIRO_ROOT)/src/miro/PositionC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.h \
 $(MIRO_ROOT)/src/miro/WorldPointC.i \
 $(MIRO_ROOT)/src/miro/PositionC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.h \
 $(MIRO_ROOT)/src/miro/TimeC.h \
 $(MIRO_ROOT)/src/miro/TimeC.i \
 $(MIRO_ROOT)/src/miro/VelocityC.h \
 $(MIRO_ROOT)/src/miro/VelocityC.i \
 $(MIRO_ROOT)/src/miro/MotionStatusC.i \
 $(MIRO_ROOT)/src/miro/OdometryC.i \
 $(MIRO_ROOT)/src/miro/ImageS.h \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.h \
 $(MIRO_ROOT)/src/miro/ExceptionS.i \
 $(MIRO_ROOT)/src/miro/TimeS.h \
 $(MIRO_ROOT)/src/miro/TimeS.i \
 $(MIRO_ROOT)/src/miro/PositionS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.h \
 $(MIRO_ROOT)/src/miro/WorldPointS.i \
 $(MIRO_ROOT)/src/miro/PositionS.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.h \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsC.i \
 $(MIRO_ROOT)/src/miro/ImageDefinitionsS.i \
 $(MIRO_ROOT)/src/miro/ImageC.h \
 $(MIRO_ROOT)/src/miro/ImageC.i \
 $(MIRO_ROOT)/src/miro/ImageS.i ImageFactory.h \
 ImageFactoryDefinitions.h MiroImage.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
