// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "NotifyLogPlayer.h"

#include "idl/MotionC.h"
#include "idl/RangeEventC.h"

#include "miro/TimeHelper.h"
#include "miro/Task.h"
#include "miro/Exception.h"

#include <ace/Arg_Shifter.h>
#include <ace/Mem_Map.h>
#include <ace/OS.h>

#include <tao/CDR.h>

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

using CosNotifyChannelAdmin::EventChannelFactory;
using CosNotifyChannelAdmin::EventChannel;

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"

CosNaming::Name channelFactoryName;

NotifyLogPlayer::NotifyLogPlayer(int argc, char *argv[], bool _colocated) :
  Super(argc, argv),
  own_(false),
  colocated_(_colocated),
  // Notification Channel
  notifyFactory_((colocated_)? 
		 TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER) :
		 resolveName<EventChannelFactory>(channelFactoryName))
{
  CosNaming::Name n(1);
  n.length(1);
  n[0].id = CORBA::string_dup("EventChannel");


  // use existing channel if possible
  if (!rebind_) {
    // get event channel
    try {
      // try resolving the event channel
      ec_ = resolveName<EventChannel>("EventChannel");
      DBG(cout << "found channel" << endl);
    }
    catch (...) {
      // if that didn't succeed create your own
      own_ = true;
      ec_ = notifyFactory_->create_channel(initialQos_, initialAdmin_, id_);
    
      // and add it to the naming service
      namingContext->bind(n, ec_.in());
    }
  }
  // create a new channel
  else {
    try {
      namingContext->unbind(n);
    }
    catch(...) {
      // well ...
    }
    own_ = true;
    ec_ = notifyFactory_->create_channel(initialQos_, initialAdmin_, id_);
    
    // and add it to the naming service
    namingContext->bind(n, ec_.in());
  }


  pPushSupplier = new Miro::StructuredPushSupplier(ec_.in(), namingContextName);

  DBG(cout << "NotifyLogPlayer initialized.." << endl);
}

NotifyLogPlayer::~NotifyLogPlayer()
{
  DBG(cout << "Destructing NotifyLogPlayer." << endl);

  delete pPushSupplier;

  try {
    if (own_) {
      CosNaming::Name n;
      n.length(1);
      n[0].id = CORBA::string_dup("EventChannel");
      namingContext->unbind(n);
    }
    if (colocated_)
      initialNamingContext->unbind(channelFactoryName);

  }
  catch (const CORBA::Exception& e) {
    cerr << "Caught CORBA exception on unbind: " << e << endl;
    cerr << "Porbably the NameSevice went down while we run." << endl;
  }

  DBG(cout << "unbound objects from Name Sevice" << endl);

  if (colocated_)
    ec_->destroy();
}

class Task : public Miro::Task
{
public:
  Task(Miro::StructuredPushSupplier * _supplier, ACE_Mem_Map& _memMap);
  virtual int svc();
  void cancel() { canceled_ = true; this->wait(); }

protected:
  Miro::StructuredPushSupplier * supplier_;
  ACE_Mem_Map& memMap_;
  bool canceled_;
};

Task::Task(Miro::StructuredPushSupplier * _supplier,
	   ACE_Mem_Map& _memMap) : 
  supplier_(_supplier),
  memMap_(_memMap),
  canceled_(false)
{}

int
Task::svc()
{
  ACE_OS::sleep(ACE_Time_Value(1));
  ACE_Time_Value timeStamp, lastStamp;
  Miro::TimeIDL timeIDL;

  CosNotification::StructuredEvent event;
  int counter = 0;

  cout << "Mmapping file." << endl;

  TAO_InputCDR istr((char*)memMap_.addr(), memMap_.size());

  cout << "Getting initial time stamp." << endl;
  istr >> timeIDL;
  Miro::timeC2A(timeIDL, timeStamp);

  cout << "Entering main loop." << endl;

  while (istr.length() != 0 && !canceled_) {
    ++ counter;

    cout << "event read ptr: " << (void*)istr.rd_ptr() << endl;
    istr >> event;
    cout << "got event: "
	 << "Domain:  " << event.header.fixed_header.event_type.domain_name << "\t"
	 << "Type:    " << event.header.fixed_header.event_type.type_name << endl;

    supplier_->sendEvent(event);
    cout << "sent event" << endl;

    if (istr.length() == 0)
      break;

    cout << "time stamp read ptr: " << (void*)istr.rd_ptr() << endl;
    istr >> timeIDL;
    lastStamp = timeStamp;
    Miro::timeC2A(timeIDL, timeStamp);

    ACE_Time_Value delta = timeStamp - lastStamp;
    cout << "sleeping: " << delta << endl;

    ACE_OS::sleep(delta);
  }

  cout << "Thread ended" << endl
       << "Send messages: " << counter << endl;
  return 0;
}


int
main(int argc, char *argv[])
{
  bool colocated = false;
  const char colocatedOpt[] = "-colocated_ec";
  
  ACE_Arg_Shifter arg_shifter (argc, argv);
  while (arg_shifter.is_anything_left ()) {
    const ACE_TCHAR *current_arg = arg_shifter.get_current ();

    if (ACE_OS::strcasecmp(current_arg, colocatedOpt) == 0) {
      arg_shifter.consume_arg();
      colocated = true;
      break;
    }

    arg_shifter.ignore_arg ();
  }

  // Init TAO Factories
  if (colocated) {
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
  }

  channelFactoryName.length(1);
  channelFactoryName[0].id = CORBA::string_dup(NOTIFY_FACTORY_NAME);

  DBG(cout << "Initialize server daemon." << endl);
  NotifyLogPlayer notifyLogPlayer(argc, argv, colocated);

  char * fileName;
  if (argc > 1) {
    fileName = argv[1];
    cout << "using file " << fileName << endl;
  }
  else {
    cout << "Please specify a file to play." << endl;
    return 1;
  }
  
  try {
    ACE_Mem_Map memoryMap(fileName);
    if (0 == memoryMap.handle())
      throw Miro::CException(errno, std::strerror(errno));
    Task * task= new Task(notifyLogPlayer.pPushSupplier, memoryMap);

    cerr << "press return to start..." << flush;
    getchar();

    task->open(0);
    notifyLogPlayer.run();
    DBG(cout << "notifyLogPlayer ended, exiting." << endl);

    task->close();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}
