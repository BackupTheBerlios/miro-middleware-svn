// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "NotifyTest.h"

#include "idl/OdometryC.h"
#include "idl/RangeEventC.h"
#include "idl/ButtonsC.h"

#include "miro/TimeHelper.h"
#include "miro/Task.h"

#include <ace/Arg_Shifter.h>
#include <ace/OS.h>

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

NotifyTest::NotifyTest(int argc, char *argv[], bool _colocated) :
  Super(argc, argv),
  colocated_(_colocated),
  // Notification Channel
  notifyFactory_((colocated_)? 
		 TAO_Notify_EventChannelFactory_i::create(poa.in()
							  ACE_ENV_ARG_PARAMETER)
		 :
		 resolveName<EventChannelFactory>(channelFactoryName)),
  id_(),
  ifgop_(CosNotifyChannelAdmin::OR_OP)
{
  try {
    ec_ = resolveName<EventChannel>("EventChannel");
    DBG(cout << "found channel" << endl);
  }
  catch (...) {
    ec_ = notifyFactory_->create_channel(initialQos_, initialAdmin_, id_);
  }

  CosNaming::Name n(1);
  try {
    if (colocated_) {
      initialNamingContext->bind(channelFactoryName, notifyFactory_.in());
      DBG(cout << "colocated factory" << flush);
    }

    n.length(1);
    n[0].id = CORBA::string_dup("EventChannel");
    namingContext->bind(n, ec_.in());

    DBG(cout << "Bound channel in naming service." << endl);
  } catch (...) {
  }

  pPushSupplier = new Miro::StructuredPushSupplier(ec_.in(),
						   namingContextName);

  DBG(cout << "NotifyTest initialized.." << endl);
}

NotifyTest::~NotifyTest()
{
  DBG(cout << "Destructing NotifyTest." << endl);

  delete pPushSupplier;

  try {
    CosNaming::Name n;
    n.length(1);
    n[0].id = CORBA::string_dup("EventChannel");
    namingContext->unbind(n);
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
  Task(Miro::StructuredPushSupplier * _supplier) : supplier_(_supplier) {}
  virtual int svc();

protected:
  Miro::StructuredPushSupplier * supplier_;
};

int
Task::svc()
{
  CosNotification::StructuredEvent motionEvent;
  CosNotification::StructuredEvent sonarEvent;
  CosNotification::StructuredEvent buttonEvent;

  Miro::MotionStatusIDL        motionStatus;
  Miro::RangeBunchEventIDL     sonarStatus;
  Miro::ButtonStatusIDL buttonStatus;
  
  buttonStatus.number = 1;
  buttonStatus.event = Miro::Button::ON_PRESS;
    
  motionEvent.header.fixed_header.event_type.domain_name = CORBA::string_dup(supplier_->domainName().c_str());
  motionEvent.header.fixed_header.event_type.type_name = CORBA::string_dup("Odometry" /*Miro::SonarEventIDL::_type_id()*/);
  motionEvent.header.fixed_header.event_name = CORBA::string_dup("");
  motionEvent.header.variable_header.length(0); // put nothing here
  motionEvent.filterable_data.length(0);

  sonarEvent.header.fixed_header.event_type.domain_name = CORBA::string_dup(supplier_->domainName().c_str());
  sonarEvent.header.fixed_header.event_type.type_name = CORBA::string_dup("Sonar" /*Miro::SonarEventIDL::_type_id()*/);
  sonarEvent.header.fixed_header.event_name = CORBA::string_dup("");
  sonarEvent.header.variable_header.length(0); // put nothing here
  sonarEvent.filterable_data.length(0);

  buttonEvent.header.fixed_header.event_type.domain_name = CORBA::string_dup(supplier_->domainName().c_str());
  buttonEvent.header.fixed_header.event_type.type_name = CORBA::string_dup("Button" /*Miro::SonarEventIDL::_type_id()*/);
  buttonEvent.header.fixed_header.event_name = CORBA::string_dup("");
  buttonEvent.header.variable_header.length(0); // put nothing here
  buttonEvent.filterable_data.length(0);

  char c = ' '; 
  while (c != 'q') {
    cout << "Choose event type:" << endl
	 << "  1 - motionEvent" << endl
	 << "  2 - sonarEvent" << endl
	 << "  3 - buttonsEvent" << endl;
    cin >> c;

    switch (c) {
    case 'q':  // quit
      break;
    case '1':  // motion Event
      timeA2C(ACE_OS::gettimeofday(), motionStatus.time);
      motionEvent.remainder_of_body <<= motionStatus;
      supplier_->sendEvent(motionEvent);
      break;
    case '2':  // sonar Event
      timeA2C(ACE_OS::gettimeofday(), sonarStatus.time);
      sonarEvent.remainder_of_body <<= sonarStatus;
      supplier_->sendEvent(sonarEvent);
      break;
    case '3':  // button Event
      timeA2C(ACE_OS::gettimeofday(), buttonStatus.time);
      buttonEvent.remainder_of_body <<= buttonStatus;
      supplier_->sendEvent(buttonEvent);
      break;
    default:
      cerr << "wrong input" << endl;
    }
  }
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

  NotifyTest notifyTest(argc, argv, colocated);
  Task task(notifyTest.pPushSupplier);

  try {
    DBG(cout << "Initialize server daemon." << endl);

    task.open(0);
    notifyTest.run();

    DBG(cout << "notifyTest ended, exiting." << endl);
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
