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
## Miro/src/pioneer/Makefile
##
##############################################################################

## this is the name of the resulting library
NAME     = pioneer

MAKEFILE = Makefile
LIB      = lib$(NAME).a
SHLIB    = lib$(NAME).$(SOEXT)


## this I didn't understand yet
# VPATH=.:Naming
# ACE_SHLIBS = -lTAO_Svc_Utils -lTAO -lACE

## specify your library files here
FILES = 	PioneerConnection \
		PioneerConsumer \
		PioneerStatus \
		PioneerMotionImpl  \
		PioneerStallImpl  \
		PioneerParameters \
		CanonPanTiltImpl \
		CanonCameraImpl \
		CanonMessage \
		GripperImpl

DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

# Only build static library 
static_libs_only = 1

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


.obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o .obj/PioneerConnection.o: PioneerConnection.cpp PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
  $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i $(MIRO_ROOT)/src/miro/Exception.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/String_Base.cpp $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h PioneerConsumer.h \
  $(TAO_ROOT)/tao/corba.h \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(MIRO_ROOT)/src/miro/DevConsumer.h PioneerStatus.h \
  $(MIRO_ROOT)/src/miro/MotionStatusC.h $(MIRO_ROOT)/src/miro/TimeC.h \
  $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/PositionC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
  $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/VelocityC.h \
  $(MIRO_ROOT)/src/miro/VelocityC.i $(MIRO_ROOT)/src/miro/MotionStatusC.i \
  $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
  $(MIRO_ROOT)/src/miro/Angle.h PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h \
  CanonPanTiltImpl.h $(MIRO_ROOT)/src/miro/CanonPanTiltS.h \
  $(MIRO_ROOT)/src/miro/PanTiltS.h $(MIRO_ROOT)/src/miro/TiltS.h \
  $(MIRO_ROOT)/src/miro/ExceptionS.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/TiltC.h \
  $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
  $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
  $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
  $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
  $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltC.i $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
  $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosMessage.h $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o .obj/PioneerConsumer.o: PioneerConsumer.cpp PioneerConsumer.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/DevConsumer.h \
  PioneerStatus.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
  $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
  $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
  $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
  $(MIRO_ROOT)/src/miro/MotionStatusC.i $(MIRO_ROOT)/src/miro/ExceptionC.h \
  $(MIRO_ROOT)/src/miro/ExceptionC.i $(MIRO_ROOT)/src/miro/Angle.h \
  PioneerStallImpl.h $(MIRO_ROOT)/src/miro/PioneerStallS.h \
  $(MIRO_ROOT)/src/miro/StallS.h $(MIRO_ROOT)/src/miro/StallC.h \
  $(MIRO_ROOT)/src/miro/StallC.i \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/StallS.i $(MIRO_ROOT)/src/miro/PioneerStallC.h \
  $(MIRO_ROOT)/src/miro/PioneerStallC.i $(MIRO_ROOT)/src/miro/PioneerStallS.i \
  PioneerParameters.h $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/DEV_Addr.i \
  $(ACE_ROOT)/ace/DEV_Connector.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h \
  $(MIRO_ROOT)/src/pioneer/CanonPanTiltImpl.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltS.h $(MIRO_ROOT)/src/miro/PanTiltS.h \
  $(MIRO_ROOT)/src/miro/TiltS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
  $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/TiltC.h \
  $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
  $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
  $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
  $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
  $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltC.i $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
  $(MIRO_ROOT)/src/miro/Exception.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h $(MIRO_ROOT)/src/miro/RangeEventC.h \
  $(MIRO_ROOT)/src/miro/RangeEventC.i $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
  $(MIRO_ROOT)/src/miro/RangeSensorS.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
  $(MIRO_ROOT)/src/miro/RangeEventS.h $(MIRO_ROOT)/src/miro/TimeS.h \
  $(MIRO_ROOT)/src/miro/TimeS.i $(MIRO_ROOT)/src/miro/RangeEventS.i \
  $(MIRO_ROOT)/src/miro/RangeSensorC.h $(MIRO_ROOT)/src/miro/RangeSensorC.i \
  $(MIRO_ROOT)/src/miro/RangeSensorS.i $(MIRO_ROOT)/src/miro/OdometryImpl.h \
  $(MIRO_ROOT)/src/miro/OdometryS.h $(MIRO_ROOT)/src/miro/PositionS.h \
  $(MIRO_ROOT)/src/miro/WorldPointS.h $(MIRO_ROOT)/src/miro/WorldPointS.i \
  $(MIRO_ROOT)/src/miro/PositionS.i $(MIRO_ROOT)/src/miro/MotionStatusS.h \
  $(MIRO_ROOT)/src/miro/VelocityS.h $(MIRO_ROOT)/src/miro/VelocityS.i \
  $(MIRO_ROOT)/src/miro/MotionStatusS.i $(MIRO_ROOT)/src/miro/OdometryC.h \
  $(MIRO_ROOT)/src/miro/OdometryC.i $(MIRO_ROOT)/src/miro/OdometryS.i \
  $(MIRO_ROOT)/src/miro/BatteryImpl.h $(MIRO_ROOT)/src/miro/BatteryS.h \
  $(MIRO_ROOT)/src/miro/BatteryC.h $(MIRO_ROOT)/src/miro/BatteryC.i \
  $(MIRO_ROOT)/src/miro/BatteryS.i $(MIRO_ROOT)/src/miro/TimeHelper.h \
  $(MIRO_ROOT)/src/psos/PsosMessage.h $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o .obj/PioneerStatus.o: PioneerStatus.cpp $(MIRO_ROOT)/src/miro/TimeHelper.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i $(MIRO_ROOT)/src/miro/TimeC.i \
  PioneerStatus.h $(MIRO_ROOT)/src/miro/MotionStatusC.h \
  $(MIRO_ROOT)/src/miro/PositionC.h $(MIRO_ROOT)/src/miro/WorldPointC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.i $(MIRO_ROOT)/src/miro/PositionC.i \
  $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
  $(MIRO_ROOT)/src/miro/MotionStatusC.i $(MIRO_ROOT)/src/miro/ExceptionC.h \
  $(MIRO_ROOT)/src/miro/ExceptionC.i $(MIRO_ROOT)/src/miro/Synch.h \
  $(MIRO_ROOT)/src/miro/Angle.h $(MIRO_ROOT)/src/psos/PsosMessage.h \
  $(MIRO_ROOT)/src/miro/DevMessage.h $(MIRO_ROOT)/src/psos/PsosDevice.h

.obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o .obj/PioneerMotionImpl.o: PioneerMotionImpl.cpp PioneerMotionImpl.h \
  $(MIRO_ROOT)/src/miro/MotionS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(MIRO_ROOT)/src/miro/ExceptionC.i \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/VelocityS.h \
  $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
  $(MIRO_ROOT)/src/miro/VelocityS.i $(MIRO_ROOT)/src/miro/MotionC.h \
  $(MIRO_ROOT)/src/miro/MotionC.i $(MIRO_ROOT)/src/miro/MotionS.i \
  $(MIRO_ROOT)/src/miro/OdometryC.h $(MIRO_ROOT)/src/miro/PositionC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
  $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/MotionStatusC.h \
  $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
  $(MIRO_ROOT)/src/miro/MotionStatusC.i $(MIRO_ROOT)/src/miro/OdometryC.i \
  $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
  $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
  $(MIRO_ROOT)/src/miro/MotionImpl.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/Synch.h \
  $(MIRO_ROOT)/src/miro/Exception.h PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h PioneerConsumer.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(MIRO_ROOT)/src/miro/DevConsumer.h PioneerStatus.h \
  $(MIRO_ROOT)/src/miro/Angle.h PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h

.obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o .obj/PioneerStallImpl.o: PioneerStallImpl.cpp PioneerStallImpl.h \
  $(MIRO_ROOT)/src/miro/PioneerStallS.h $(MIRO_ROOT)/src/miro/StallS.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i $(MIRO_ROOT)/src/miro/StallC.i \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/StallS.i $(MIRO_ROOT)/src/miro/PioneerStallC.h \
  $(MIRO_ROOT)/src/miro/PioneerStallC.i $(MIRO_ROOT)/src/miro/PioneerStallS.i

.obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o .obj/PioneerParameters.o: PioneerParameters.cpp PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/String_Base.cpp $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i $(ACE_ROOT)/ace/DEV_Connector.h \
  $(ACE_ROOT)/ace/DEV_IO.h $(ACE_ROOT)/ace/DEV.h \
  $(ACE_ROOT)/ace/IO_SAP.h $(ACE_ROOT)/ace/IO_SAP.i \
  $(ACE_ROOT)/ace/DEV.i $(ACE_ROOT)/ace/DEV_IO.i \
  $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h \
  $(MIRO_ROOT)/src/miro/Synch.h $(MIRO_ROOT)/src/miro/RangeSensorC.h \
  $(MIRO_ROOT)/src/miro/ExceptionC.h $(MIRO_ROOT)/src/miro/ExceptionC.i \
  $(MIRO_ROOT)/src/miro/RangeEventC.h $(MIRO_ROOT)/src/miro/TimeC.h \
  $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/RangeEventC.i \
  $(MIRO_ROOT)/src/miro/RangeSensorC.i \
  $(MIRO_ROOT)/src/miro/ScanDescriptionHelper.h $(MIRO_ROOT)/src/miro/Angle.h \
  \
  \
  \
 

.obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o .obj/CanonPanTiltImpl.o: CanonPanTiltImpl.cpp \
  $(MIRO_ROOT)/src/pioneer/CanonPanTiltImpl.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltS.h $(MIRO_ROOT)/src/miro/PanTiltS.h \
  $(MIRO_ROOT)/src/miro/TiltS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(MIRO_ROOT)/src/miro/ExceptionC.i \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/TiltC.h \
  $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
  $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
  $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
  $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
  $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltC.i $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
  $(MIRO_ROOT)/src/miro/Exception.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConsumer.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(MIRO_ROOT)/src/miro/DevConsumer.h \
  $(MIRO_ROOT)/src/pioneer/PioneerStatus.h \
  $(MIRO_ROOT)/src/miro/MotionStatusC.h $(MIRO_ROOT)/src/miro/TimeC.h \
  $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/PositionC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
  $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/VelocityC.h \
  $(MIRO_ROOT)/src/miro/VelocityC.i $(MIRO_ROOT)/src/miro/MotionStatusC.i \
  $(MIRO_ROOT)/src/miro/Angle.h $(MIRO_ROOT)/src/pioneer/PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h

.obj/CanonCameraImpl.o .obj/CanonCameraImpl.o .obj/CanonCameraImpl.o .obj/CanonCameraImpl.o: CanonCameraImpl.cpp \
  $(MIRO_ROOT)/src/pioneer/CanonCameraImpl.h \
  $(MIRO_ROOT)/src/miro/CanonCameraS.h $(MIRO_ROOT)/src/miro/ExceptionS.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Log_Record.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Malloc.i $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.h $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp $(ACE_ROOT)/ace/SString.i \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object.i $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/NVList.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(TAO_ROOT)/tao/NVList.i \
  $(TAO_ROOT)/tao/LocalObject.h \
  $(TAO_ROOT)/tao/LocalObject.i \
  $(TAO_ROOT)/tao/Principal.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/Principal.i $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Services.h \
  $(TAO_ROOT)/tao/Services.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/DomainC.h \
  $(TAO_ROOT)/tao/DomainC.i \
  $(TAO_ROOT)/tao/WrongTransactionC.h \
  $(TAO_ROOT)/tao/WrongTransactionC.i \
  $(TAO_ROOT)/tao/Object_KeyC.h \
  $(TAO_ROOT)/tao/Object_KeyC.i \
  $(TAO_ROOT)/tao/ObjectIDList.h \
  $(TAO_ROOT)/tao/ObjectIDList.i \
  $(TAO_ROOT)/tao/StringSeqC.h \
  $(TAO_ROOT)/tao/StringSeqC.i \
  $(MIRO_ROOT)/src/miro/ExceptionC.i \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(MIRO_ROOT)/src/miro/ExceptionS.i $(MIRO_ROOT)/src/miro/CanonCameraC.h \
  $(MIRO_ROOT)/src/miro/CanonCameraC.i $(MIRO_ROOT)/src/miro/CanonCameraS.i \
  $(MIRO_ROOT)/src/miro/Exception.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h $(MIRO_ROOT)/src/miro/Synch.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConsumer.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(MIRO_ROOT)/src/miro/DevConsumer.h \
  $(MIRO_ROOT)/src/pioneer/PioneerStatus.h \
  $(MIRO_ROOT)/src/miro/MotionStatusC.h $(MIRO_ROOT)/src/miro/TimeC.h \
  $(MIRO_ROOT)/src/miro/TimeC.i $(MIRO_ROOT)/src/miro/PositionC.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
  $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/VelocityC.h \
  $(MIRO_ROOT)/src/miro/VelocityC.i $(MIRO_ROOT)/src/miro/MotionStatusC.i \
  $(MIRO_ROOT)/src/miro/Angle.h $(MIRO_ROOT)/src/pioneer/PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i $(MIRO_ROOT)/src/miro/Singleton.h

.obj/CanonMessage.o .obj/CanonMessage.o .obj/CanonMessage.o .obj/CanonMessage.o: CanonMessage.cpp CanonMessage.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/svc_export.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Export.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i CanonDevice.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
