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
## Miro/src/pioneerBase/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = pioneer psos miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS += -lmiro

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= PioneerBase

## additional files
FILES	= PioneerBase

SRC	= $(addsuffix .cpp,$(FILES))
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

$(BIN): $(STATIC_DEPS)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/PioneerBase.o .obj/PioneerBase.so .shobj/PioneerBase.o .shobj/PioneerBase.so: PioneerBase.cpp PioneerBase.h $(MIRO_ROOT)/src/miro/Server.h \
  $(MIRO_ROOT)/src/miro/Client.h $(TAO_ROOT)/tao/corba.h \
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
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(ACE_ROOT)/ace/INET_Addr.h $(ACE_ROOT)/ace/Addr.h \
  $(ACE_ROOT)/ace/Addr.i $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
  $(ACE_ROOT)/ace/SOCK_Dgram.h $(ACE_ROOT)/ace/SOCK.h \
  $(ACE_ROOT)/ace/IPC_SAP.h $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i $(ACE_ROOT)/ace/SOCK_Dgram.i \
  $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
  $(ACE_ROOT)/ace/Reactor.h $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Reactor.i $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Context_Interface.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op.cpp \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
  $(MIRO_ROOT)/src/miro/Exception.h $(MIRO_ROOT)/src/miro/RobotParameters.h \
  $(MIRO_ROOT)/src/miro/Parameters.h $(MIRO_ROOT)/src/miro/Singleton.h \
  $(MIRO_ROOT)/src/miro/Synch.h $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Containers.h $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Singleton.h $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(ACE_ROOT)/ace/Thread_Manager.i $(ACE_ROOT)/ace/Task.i \
  $(ACE_ROOT)/ace/Task_T.h $(ACE_ROOT)/ace/Message_Queue.h \
  $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
  $(ACE_ROOT)/ace/Message_Queue_T.h \
  $(ACE_ROOT)/ace/Message_Queue_T.i \
  $(ACE_ROOT)/ace/Message_Queue_T.cpp \
  $(ACE_ROOT)/ace/Notification_Strategy.h \
  $(ACE_ROOT)/ace/Notification_Strategy.inl \
  $(ACE_ROOT)/ace/Message_Queue.i $(ACE_ROOT)/ace/Task_T.i \
  $(ACE_ROOT)/ace/Task_T.cpp $(ACE_ROOT)/ace/Module.h \
  $(ACE_ROOT)/ace/Module.i $(ACE_ROOT)/ace/Module.cpp \
  $(ACE_ROOT)/ace/Stream_Modules.h \
  $(ACE_ROOT)/ace/Stream_Modules.cpp \
  $(MIRO_ROOT)/src/miro/ReactorTask.h $(ACE_ROOT)/ace/Sched_Params.i \
  $(MIRO_ROOT)/src/miro/StructuredPushSupplier.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/notify_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotificationS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyFilterS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosEventChannelAdminS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS.i \
  $(MIRO_ROOT)/src/miro/OdometryImpl.h $(MIRO_ROOT)/src/miro/OdometryS.h \
  $(MIRO_ROOT)/src/miro/ExceptionS.h $(MIRO_ROOT)/src/miro/ExceptionC.h \
  $(MIRO_ROOT)/src/miro/ExceptionC.i $(MIRO_ROOT)/src/miro/ExceptionS.i \
  $(MIRO_ROOT)/src/miro/PositionS.h $(MIRO_ROOT)/src/miro/WorldPointS.h \
  $(MIRO_ROOT)/src/miro/WorldPointC.h $(MIRO_ROOT)/src/miro/WorldPointC.i \
  $(MIRO_ROOT)/src/miro/WorldPointS.i $(MIRO_ROOT)/src/miro/PositionC.h \
  $(MIRO_ROOT)/src/miro/PositionC.i $(MIRO_ROOT)/src/miro/PositionS.i \
  $(MIRO_ROOT)/src/miro/MotionStatusS.h $(MIRO_ROOT)/src/miro/TimeS.h \
  $(MIRO_ROOT)/src/miro/TimeC.h $(MIRO_ROOT)/src/miro/TimeC.i \
  $(MIRO_ROOT)/src/miro/TimeS.i $(MIRO_ROOT)/src/miro/VelocityS.h \
  $(MIRO_ROOT)/src/miro/VelocityC.h $(MIRO_ROOT)/src/miro/VelocityC.i \
  $(MIRO_ROOT)/src/miro/VelocityS.i $(MIRO_ROOT)/src/miro/MotionStatusC.h \
  $(MIRO_ROOT)/src/miro/MotionStatusC.i $(MIRO_ROOT)/src/miro/MotionStatusS.i \
  $(MIRO_ROOT)/src/miro/OdometryC.h $(MIRO_ROOT)/src/miro/OdometryC.i \
  $(MIRO_ROOT)/src/miro/OdometryS.i $(MIRO_ROOT)/src/miro/BatteryImpl.h \
  $(MIRO_ROOT)/src/miro/BatteryS.h $(MIRO_ROOT)/src/miro/BatteryC.h \
  $(MIRO_ROOT)/src/miro/BatteryC.i $(MIRO_ROOT)/src/miro/BatteryS.i \
  $(MIRO_ROOT)/src/psos/PsosEventHandler.h \
  $(MIRO_ROOT)/src/miro/DevEventHandler.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConnection.h \
  $(MIRO_ROOT)/src/psos/PsosConnection.h \
  $(MIRO_ROOT)/src/miro/TtyConnection.h $(ACE_ROOT)/ace/DEV_IO.h \
  $(ACE_ROOT)/ace/DEV.h $(ACE_ROOT)/ace/IO_SAP.h \
  $(ACE_ROOT)/ace/IO_SAP.i $(ACE_ROOT)/ace/DEV_Addr.h \
  $(ACE_ROOT)/ace/DEV_Addr.i $(ACE_ROOT)/ace/DEV.i \
  $(ACE_ROOT)/ace/DEV_IO.i $(ACE_ROOT)/ace/DEV_Connector.i \
  $(MIRO_ROOT)/src/pioneer/CanonMessage.h $(MIRO_ROOT)/src/miro/DevMessage.h \
  $(MIRO_ROOT)/src/pioneer/CanonDevice.h \
  $(MIRO_ROOT)/src/pioneer/PioneerConsumer.h \
  $(MIRO_ROOT)/src/miro/DevConsumer.h \
  $(MIRO_ROOT)/src/pioneer/PioneerStatus.h $(MIRO_ROOT)/src/miro/Angle.h \
  $(MIRO_ROOT)/src/pioneer/PioneerMotionImpl.h \
  $(MIRO_ROOT)/src/miro/MotionS.h $(MIRO_ROOT)/src/miro/MotionC.h \
  $(MIRO_ROOT)/src/miro/MotionC.i $(MIRO_ROOT)/src/miro/MotionS.i \
  $(MIRO_ROOT)/src/miro/DifferentialMotionImpl.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionS.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionC.h \
  $(MIRO_ROOT)/src/miro/DifferentialMotionC.i \
  $(MIRO_ROOT)/src/miro/DifferentialMotionS.i \
  $(MIRO_ROOT)/src/miro/MotionImpl.h $(MIRO_ROOT)/src/miro/MotionParameters.h \
  $(MIRO_ROOT)/src/pioneer/PioneerStallImpl.h \
  $(MIRO_ROOT)/src/miro/PioneerStallS.h $(MIRO_ROOT)/src/miro/StallS.h \
  $(MIRO_ROOT)/src/miro/StallC.h $(MIRO_ROOT)/src/miro/StallC.i \
  $(MIRO_ROOT)/src/miro/StallS.i $(MIRO_ROOT)/src/miro/PioneerStallC.h \
  $(MIRO_ROOT)/src/miro/PioneerStallC.i $(MIRO_ROOT)/src/miro/PioneerStallS.i \
  $(MIRO_ROOT)/src/pioneer/CanonPanTiltImpl.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltS.h $(MIRO_ROOT)/src/miro/PanTiltS.h \
  $(MIRO_ROOT)/src/miro/TiltS.h $(MIRO_ROOT)/src/miro/TiltC.h \
  $(MIRO_ROOT)/src/miro/TiltC.i $(MIRO_ROOT)/src/miro/TiltS.i \
  $(MIRO_ROOT)/src/miro/PanS.h $(MIRO_ROOT)/src/miro/PanC.h \
  $(MIRO_ROOT)/src/miro/PanC.i $(MIRO_ROOT)/src/miro/PanS.i \
  $(MIRO_ROOT)/src/miro/PanTiltC.h $(MIRO_ROOT)/src/miro/PanTiltC.i \
  $(MIRO_ROOT)/src/miro/PanTiltS.i $(MIRO_ROOT)/src/miro/CanonPanTiltC.h \
  $(MIRO_ROOT)/src/miro/CanonPanTiltC.i $(MIRO_ROOT)/src/miro/CanonPanTiltS.i \
  $(MIRO_ROOT)/src/pioneer/CanonCameraImpl.h \
  $(MIRO_ROOT)/src/miro/CanonCameraS.h $(MIRO_ROOT)/src/miro/CanonCameraC.h \
  $(MIRO_ROOT)/src/miro/CanonCameraC.i $(MIRO_ROOT)/src/miro/CanonCameraS.i \
  $(MIRO_ROOT)/src/miro/RangeSensorImpl.h \
  $(MIRO_ROOT)/src/miro/RangeSensorS.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionS.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.h \
  $(MIRO_ROOT)/src/miro/ScanDescriptionC.i \
  $(MIRO_ROOT)/src/miro/ScanDescriptionS.i \
  $(MIRO_ROOT)/src/miro/RangeEventS.h $(MIRO_ROOT)/src/miro/RangeEventC.h \
  $(MIRO_ROOT)/src/miro/RangeEventC.i $(MIRO_ROOT)/src/miro/RangeEventS.i \
  $(MIRO_ROOT)/src/miro/RangeSensorC.h $(MIRO_ROOT)/src/miro/RangeSensorC.i \
  $(MIRO_ROOT)/src/miro/RangeSensorS.i $(MIRO_ROOT)/src/miro/Utils.h \
  \
  \
  \
  $(MIRO_ROOT)/src/pioneer/PioneerParameters.h \
  $(MIRO_ROOT)/src/psos/PsosParameters.h \
  $(MIRO_ROOT)/src/miro/TtyParameters.h $(MIRO_ROOT)/src/miro/DevParameters.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_ID_Pool_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_ID_Pool_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_ID_Pool_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Channel_Objects_Factory.h \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_POA_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Collection_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Collection.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/ESF/ESF_Worker.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/ESF/ESF_Worker.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/ESF/ESF_Worker.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Event.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/tao/TimeBaseC.h \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
  $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
  $(TAO_ROOT)/tao/TAO_Singleton.h \
  $(TAO_ROOT)/tao/TAO_Singleton.inl \
  $(TAO_ROOT)/tao/TAO_Singleton.cpp \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
  $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.inl \
  $(TAO_ROOT)/tao/TimeBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Event.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Listeners.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Event_Manager_Objects_Factory.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
