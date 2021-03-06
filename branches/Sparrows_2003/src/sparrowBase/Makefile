## -*- Makefile -*- ##########################################################
##
## This file is part of Miro (The Middleware For Robots)
##
## (c) 2000, 2001, 2002, 2003
## Department of Neural Information Processing, University of Ulm, Germany
##
## $Id$
## $Date$
## 
##
## Miro/src/sparrowBase/Makefile
##
##############################################################################

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INSTALL = $(VBIN:%=$(INSBIN)/%)

## static libraries

STATIC_LIBS = sparrow can pioneer psos faulMotor faulTty miroSvc

## performing the magic

STATIC_DEPS = $(addprefix $(MIRO_ROOT)/lib/lib, $(addsuffix .a,$(STATIC_LIBS)))
LDLIBS += $(addprefix -l, $(STATIC_LIBS))

## dynamic libraries if needed
LDLIBS +=	-lmiro 

## program to be built
## there has to exist a source file with the same name (mybinary.cc)
BIN 	= SparrowBase

## additional files
FILES	= \
	SparrowServer \
	SparrowBase

## performing the magic

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


.obj/SparrowServer.o .obj/SparrowServer.so .shobj/SparrowServer.o .shobj/SparrowServer.so: SparrowServer.cpp SparrowServer.h \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
  $(ACE_ROOT)/ace/pre.h $(TAO_ROOT)/tao/corba.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/Log_Priority.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/Handle_Ops.h $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
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
  $(ACE_ROOT)/ace/SString.i $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
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
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/AbstractBase.h \
  $(TAO_ROOT)/tao/AbstractBase.inl \
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
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/nsconf.h \
  \
  \
  \
  \
  \
  \
  \
  \
  $(ACE_ROOT)/ace/TTY_IO.h \
  $(ACE_ROOT)/ace/DEV_IO.h $(ACE_ROOT)/ace/DEV.h \
  $(ACE_ROOT)/ace/IO_SAP.h $(ACE_ROOT)/ace/IO_SAP.i \
  $(ACE_ROOT)/ace/DEV_Addr.h $(ACE_ROOT)/ace/DEV_Addr.i \
  $(ACE_ROOT)/ace/DEV.i $(ACE_ROOT)/ace/DEV_IO.i \
  \
  $(ACE_ROOT)/ace/Task.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
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
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
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
  \
  $(ACE_ROOT)/ace/Sched_Params.h \
  $(ACE_ROOT)/ace/Sched_Params.i \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS.h \
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
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/ExceptionC.i \
  /home/rr4/Miro/src/miro/ExceptionS.i \
  \
  \
  /home/rr4/Miro/src/miro/ScanDescriptionC.i \
  /home/rr4/Miro/src/miro/ScanDescriptionS.i \
  \
  /home/rr4/Miro/src/miro/TimeC.i \
  /home/rr4/Miro/src/miro/TimeS.i \
  /home/rr4/Miro/src/miro/RangeEventC.i \
  /home/rr4/Miro/src/miro/RangeEventS.i \
  \
  /home/rr4/Miro/src/miro/RangeSensorC.i \
  /home/rr4/Miro/src/miro/RangeSensorS.i \
  \
  \
  \
  $(ACE_ROOT)/ace/Date_Time.h $(ACE_ROOT)/ace/Date_Time.i \
  \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/WorldPointC.i \
  /home/rr4/Miro/src/miro/WorldPointS.i \
  /home/rr4/Miro/src/miro/PositionC.i \
  /home/rr4/Miro/src/miro/PositionS.i \
  \
  \
  /home/rr4/Miro/src/miro/VelocityC.i /home/rr4/Miro/src/miro/VelocityS.i \
  \
  /home/rr4/Miro/src/miro/MotionStatusC.i \
  /home/rr4/Miro/src/miro/MotionStatusS.i \
  /home/rr4/Miro/src/miro/OdometryC.i \
  /home/rr4/Miro/src/miro/OdometryS.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  $(ACE_ROOT)/ace/DEV_Connector.h \
  $(ACE_ROOT)/ace/DEV_Connector.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/MotionC.i /home/rr4/Miro/src/miro/MotionS.i \
  \
  /home/rr4/Miro/src/miro/DifferentialMotionC.i \
  /home/rr4/Miro/src/miro/DifferentialMotionS.i \
  \
  /home/rr4/Miro/src/miro/SparrowMotionC.i \
  /home/rr4/Miro/src/miro/SparrowMotionS.i \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/KickerC.i /home/rr4/Miro/src/miro/KickerS.i \
  \
  \
  /home/rr4/Miro/src/miro/ButtonsC.i /home/rr4/Miro/src/miro/ButtonsS.i \
  \
  \
  /home/rr4/Miro/src/miro/StallC.i /home/rr4/Miro/src/miro/StallS.i \
  \
  \
  \
  /home/rr4/Miro/src/miro/PanC.i /home/rr4/Miro/src/miro/PanS.i \
  \
  /home/rr4/Miro/src/miro/SparrowPanTiltC.i \
  /home/rr4/Miro/src/miro/SparrowPanTiltS.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  $(TAO_ROOT)/tao/ORB_Core.h \
  $(TAO_ROOT)/tao/Policy_Manager.h \
  $(TAO_ROOT)/tao/Policy_Set.h \
  $(TAO_ROOT)/tao/Policy_Set.i \
  $(TAO_ROOT)/tao/debug.h \
  $(TAO_ROOT)/tao/Policy_Manager.i \
  $(TAO_ROOT)/tao/Resource_Factory.h \
  $(TAO_ROOT)/tao/params.h $(TAO_ROOT)/tao/params.i \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
  $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
  $(TAO_ROOT)/tao/TAO_Singleton.h \
  $(TAO_ROOT)/tao/TAO_Singleton.inl \
  $(TAO_ROOT)/tao/TAO_Singleton.cpp \
  $(TAO_ROOT)/tao/Adapter.h \
  $(TAO_ROOT)/tao/Adapter.i \
  $(TAO_ROOT)/tao/PolicyFactory_Registry.h \
  $(TAO_ROOT)/tao/PortableInterceptorC.h \
  $(TAO_ROOT)/tao/DynamicC.h \
  $(TAO_ROOT)/tao/DynamicC.i \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.h \
  $(TAO_ROOT)/tao/ObjectReferenceTemplateC.i \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.h \
  $(TAO_ROOT)/tao/Messaging_SyncScopeC.i \
  $(TAO_ROOT)/tao/IOPC.h $(TAO_ROOT)/tao/IOPC.i \
  $(TAO_ROOT)/tao/PortableInterceptorC.i \
  $(ACE_ROOT)/ace/Map_Manager.h \
  $(ACE_ROOT)/ace/Map_Manager.i \
  $(ACE_ROOT)/ace/Map_Manager.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Service_Types.h \
  $(ACE_ROOT)/ace/Service_Types.i \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(TAO_ROOT)/tao/Parser_Registry.h \
  $(TAO_ROOT)/tao/Parser_Registry.i \
  $(TAO_ROOT)/tao/Service_Callbacks.h \
  $(TAO_ROOT)/tao/Service_Callbacks.i \
  $(TAO_ROOT)/tao/Fault_Tolerance_Service.h \
  $(TAO_ROOT)/tao/Fault_Tolerance_Service.i \
  $(TAO_ROOT)/tao/Cleanup_Func_Registry.h \
  $(TAO_ROOT)/tao/Cleanup_Func_Registry.inl \
  $(TAO_ROOT)/tao/Object_Ref_Table.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
  $(ACE_ROOT)/ace/Functor.h $(ACE_ROOT)/ace/Functor.i \
  $(ACE_ROOT)/ace/Functor_T.h $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
  $(TAO_ROOT)/tao/Interceptor_List.h \
  $(TAO_ROOT)/tao/Interceptor_List.inl \
  $(TAO_ROOT)/tao/PICurrent.h \
  $(TAO_ROOT)/tao/ORB_Core.i \
  \
  \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_EventChannelFactory_i.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_CO_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_POA_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_Collection_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_EMO_Factory.h

.obj/SparrowBase.o .obj/SparrowBase.so .shobj/SparrowBase.o .shobj/SparrowBase.so: SparrowBase.cpp SparrowServer.h \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
  $(ACE_ROOT)/ace/pre.h $(TAO_ROOT)/tao/corba.h \
  $(ACE_ROOT)/ace/post.h $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(TAO_ROOT)/tao/corbafwd.h $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Message_Block.h $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/corbafwd.i \
  $(TAO_ROOT)/tao/Typecode.h $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/Log_Priority.h \
  $(TAO_ROOT)/tao/Exception.h $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/Handle_Ops.h $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
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
  $(ACE_ROOT)/ace/SString.i $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/Typecode.i $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/CDR.h $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Object.h \
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
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/Encodable.h \
  $(TAO_ROOT)/tao/PolicyC.i $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/BoundsC.h \
  $(TAO_ROOT)/tao/BoundsC.i \
  $(TAO_ROOT)/tao/ValueBase.h \
  $(TAO_ROOT)/tao/ValueBase.i \
  $(TAO_ROOT)/tao/ValueFactory.h \
  $(TAO_ROOT)/tao/ValueFactory.i \
  $(TAO_ROOT)/tao/AbstractBase.h \
  $(TAO_ROOT)/tao/AbstractBase.inl \
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
  $(ACE_ROOT)/ace/Atomic_Op.h $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
  $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
  $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
  $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/nsconf.h \
  \
  \
  \
  \
  \
  \
  \
  \
  $(ACE_ROOT)/ace/TTY_IO.h \
  $(ACE_ROOT)/ace/DEV_IO.h $(ACE_ROOT)/ace/DEV.h \
  $(ACE_ROOT)/ace/IO_SAP.h $(ACE_ROOT)/ace/IO_SAP.i \
  $(ACE_ROOT)/ace/DEV_Addr.h $(ACE_ROOT)/ace/DEV_Addr.i \
  $(ACE_ROOT)/ace/DEV.i $(ACE_ROOT)/ace/DEV_IO.i \
  \
  $(ACE_ROOT)/ace/Task.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
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
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
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
  \
  $(ACE_ROOT)/ace/Sched_Params.h \
  $(ACE_ROOT)/ace/Sched_Params.i \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS.h \
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
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/ExceptionC.i \
  /home/rr4/Miro/src/miro/ExceptionS.i \
  \
  \
  /home/rr4/Miro/src/miro/ScanDescriptionC.i \
  /home/rr4/Miro/src/miro/ScanDescriptionS.i \
  \
  /home/rr4/Miro/src/miro/TimeC.i \
  /home/rr4/Miro/src/miro/TimeS.i \
  /home/rr4/Miro/src/miro/RangeEventC.i \
  /home/rr4/Miro/src/miro/RangeEventS.i \
  \
  /home/rr4/Miro/src/miro/RangeSensorC.i \
  /home/rr4/Miro/src/miro/RangeSensorS.i \
  \
  \
  \
  $(ACE_ROOT)/ace/Date_Time.h $(ACE_ROOT)/ace/Date_Time.i \
  \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/WorldPointC.i \
  /home/rr4/Miro/src/miro/WorldPointS.i \
  /home/rr4/Miro/src/miro/PositionC.i \
  /home/rr4/Miro/src/miro/PositionS.i \
  \
  \
  /home/rr4/Miro/src/miro/VelocityC.i /home/rr4/Miro/src/miro/VelocityS.i \
  \
  /home/rr4/Miro/src/miro/MotionStatusC.i \
  /home/rr4/Miro/src/miro/MotionStatusS.i \
  /home/rr4/Miro/src/miro/OdometryC.i \
  /home/rr4/Miro/src/miro/OdometryS.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  $(ACE_ROOT)/ace/DEV_Connector.h \
  $(ACE_ROOT)/ace/DEV_Connector.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/MotionC.i /home/rr4/Miro/src/miro/MotionS.i \
  \
  /home/rr4/Miro/src/miro/DifferentialMotionC.i \
  /home/rr4/Miro/src/miro/DifferentialMotionS.i \
  \
  /home/rr4/Miro/src/miro/SparrowMotionC.i \
  /home/rr4/Miro/src/miro/SparrowMotionS.i \
  \
  \
  \
  \
  /home/rr4/Miro/src/miro/KickerC.i /home/rr4/Miro/src/miro/KickerS.i \
  \
  \
  /home/rr4/Miro/src/miro/ButtonsC.i /home/rr4/Miro/src/miro/ButtonsS.i \
  \
  \
  /home/rr4/Miro/src/miro/StallC.i /home/rr4/Miro/src/miro/StallS.i \
  \
  \
  \
  /home/rr4/Miro/src/miro/PanC.i /home/rr4/Miro/src/miro/PanS.i \
  \
  /home/rr4/Miro/src/miro/SparrowPanTiltC.i \
  /home/rr4/Miro/src/miro/SparrowPanTiltS.i \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_EventChannelFactory_i.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_CO_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_POA_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_Collection_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Notify/Notify_Default_EMO_Factory.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
