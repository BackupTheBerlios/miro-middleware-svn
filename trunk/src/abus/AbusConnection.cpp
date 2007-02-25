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
#include "miro/Exception.h"

#include "AbusConnection.h"
#include "AbusDevice.h"
#include "AbusMessage.h"

#include <iostream>
#include <miro/Log.h>
#include <unistd.h>

using std::endl;
using std::string;

using Miro::Exception;
using Miro::CException;
using Miro::ACE_Exception;
using Miro::DevParameters;

namespace Abus
{
  //
  // Constructors / Destructors
  //

  Connection::Connection(ACE_Reactor* _reactor,
			 Miro::DevEventHandler* _eventHandler,
			 const DevParameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters)
  {
    MIRO_LOG(LL_NOTICE, "AbusConnection intizialising\n");

    // start polling for devices
    linkRequest("RWI-DEV", "MSP", "1");
  }

  Connection::~Connection()
  {
    MIRO_LOG(LL_NOTICE, "Destructing AbusConnection\n");
  }

  //--------------------------------------------------------------------------
  // send messages and data to the abus manager
  //--------------------------------------------------------------------------

  void
  Connection::reset() throw(Exception)
  {
    Message msg(D2M_RESET, 0, 0, 0);

    sendMessage(&msg);
  }

  void
  Connection::linkRequest(const char *prot, const char *type, 
			  const char *model)  throw(Exception)
  {
    Message msg(D2M_LINK_REQUEST, 0, 0, 0);

    DeviceProtocol* protocol = (DeviceProtocol*)msg.message();

    strncpy(protocol->prot(), prot, DeviceProtocol::PROT_LEN);
    strncpy(protocol->type(), type, DeviceProtocol::TYPE_LEN);
    strncpy(protocol->model(), model, DeviceProtocol::MODEL_LEN);

    msg.msgLen() += sizeof(DeviceProtocol);
    sendMessage(&msg);
  }

  void
  Connection::linkApprove(unsigned char devId, 
			  unsigned char approve) throw(Exception)
  {
    Message msg(D2M_LINK_APPROVE, approve, devId, 0);
    int retval = 0;

    //  retval = sendMessage(&msg);
    sendMessage(&msg);
    if (retval) {
      return;
    }

    if (approve && (devId != 0)) 
      deviceTable[devId].state = ACK;
    else 
      deviceTable[devId].state = NOT;
  }

  void 
  Connection::getLongId(unsigned char devId) throw(Exception) 
  {
    Message msg(D2M_GET_LONG_ID, 0, devId, 0);

    sendMessage(&msg);
  }

  void
  Connection::getId(unsigned char devId) throw(Exception)
  {
    Message msg(D2M_GET_ID, 0, devId, 0);

    sendMessage(&msg);
  }

  void
  Connection::getType(unsigned char devId) throw(Exception) 
  {
    Message msg(D2M_GET_TYPE, 0, devId, 0);

    sendMessage(&msg);
  }

  void
  Connection::getStatus(unsigned char devId) throw(Exception) 
  {
    Message msg(D2M_GET_STATUS, 0, devId, 0);

    sendMessage(&msg);
  }

  void
  Connection::enable(unsigned char devId, 
		     unsigned char enable) throw(Exception) 
  {
    Message msg(D2M_ENABLE, enable, devId, 0);

    sendMessage(&msg);

    //  _HACK_ !!!!  There is a race with the manager here!
    //  _GROSS_ !!!

    //  This message goes to the manager which does 
    //  some book keeping and then sets the enable 
    //  flages in the kernel and then sends a message
    //  to the controller card.  If this client attempts
    //  to send a message to the device on the bus before
    //  the manager has finished its work, the send will fail.

    usleep((unsigned long)(.05*1000000));
  }

  void
  Connection::disconnect(unsigned char devId) throw(Exception) 
  {
    Message msg(D2M_DISCONNECT, 0, devId, 0);

    deviceTable[devId].state = NOT;

    sendMessage(&msg);
  }

  void
  Connection::sendMessage(const Message* msg) throw(Exception)
  {
    if ((deviceTable[msg->devId()].state == NOT) && (msg->devId()!=0)) {
      throw Exception(std::string("non-existent devId "));
    }

    MIRO_DBG_OSTR(B21,LL_PRATTLE,"abus: write() - dev 0x" << std::hex << (int)msg->devId() << std::dec << endl);

    for (int i = 0; i < 3; ++i) {
      if (ioBuffer_.send_n(msg->buffer(), Message::MSG_LEN) >= 0) {
	return;
      }
      MIRO_DBG_OSTR(B21,LL_PRATTLE, "abus: write() failed - " << (i+1) << endl);

      ACE_OS::sleep(ACE_Time_Value(0, (unsigned long)(.01*1000000)));
    }
    MIRO_LOG_OSTR(LL_ERROR, "abus: write() failed - giving up" << endl);
  }
};




