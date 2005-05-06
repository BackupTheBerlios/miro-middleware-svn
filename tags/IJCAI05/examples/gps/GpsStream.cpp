/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#include "GpsStream.h"

#include "miro/Server.h"
#include "miro/IO.h"
#include "idl/GpsDataC.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

GpsStream::GpsStream(EventChannel_ptr _ec,
		     const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added;
  added.length(3);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("GpsPosition");
  added[1].domain_name =  CORBA::string_dup(domainName.c_str());
  added[1].type_name = CORBA::string_dup("GpsDilution");
  added[2].domain_name =  CORBA::string_dup(domainName.c_str());
  added[2].type_name = CORBA::string_dup("GpsSentence");

  setSubscriptions(added);
}

GpsStream::~GpsStream()
{
}

void
GpsStream::push_structured_event(const StructuredEvent & notification
				 ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::GpsPositionEventIDL * pPosition;
  const Miro::GpsDilutionEventIDL * pDilution;
  const Miro::GpsSentenceEventIDL * pSentence;

  if (notification.remainder_of_body >>= pPosition)
    cout << *pPosition << endl;
  else if (notification.remainder_of_body >>= pDilution)
    cout << *pDilution << endl;
  else if (notification.remainder_of_body >>= pSentence)
    cout << *pSentence << endl;
  else
    cerr << "No GPS event in message." << endl;
}

int
main(int argc, char *argv[])
{
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));

    cerr << "press return to start..." << flush;
    getchar();

    // The consumer that gets the events
    GpsStream pushConsumer(ec.in(), server.namingContextName);

    cerr << "Loop forever handling events." << endl;
    server.run();
    cerr << "Server stoped, exiting." << endl;

    pushConsumer.disconnect();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception!" << endl;
    return 1;
  }
  return 0;
}

