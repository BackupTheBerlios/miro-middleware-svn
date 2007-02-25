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
#include "BaseEventHandler.h"

#include "BaseConsumer.h"
#include "BaseConnection.h"
#include "Parameters.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;

namespace Base
{
  EventHandler::EventHandler(Connection& _connection, 
			     Consumer& _consumer,
			     const Parameters& _parameters) :
    connection(_connection),
    consumer(_consumer),
    parameters(_parameters)
  {
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing BaseEventHandler." << endl);
  }

  int 
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *)
  {
    DBG(cout << "Battery polling timer: timeout." << endl);

    static int warnings = 0;
    double currentVoltage;

    // the first time, current voltage will be zero, 
    // but we will not believe this

    consumer.batteryVoltageMutex.acquire();
    currentVoltage = consumer.batteryVoltage;
    consumer.batteryVoltageMutex.release();

    DBG(cout << "Battery voltage:" << currentVoltage << endl);

    if ((currentVoltage > MIN_BELIEVABLE_VOLTAGE) &&
	(currentVoltage < MAX_BELIEVABLE_VOLTAGE)) {
      if (currentVoltage < parameters.voltWarn) {
	cerr << "Base Voltage Getting Dangerously Low: " << currentVoltage << "V" << endl;
	system("echo Base Voltage Getting Dangerously Low | wall");
      }
      else if (currentVoltage < parameters.voltPanic) {
	cerr << "Base Voltage CRITICALLY Low" << endl
	     << "Recharge the base" << endl;
	++warnings;
	system("echo Base Voltage CRITICALLY Low! Recharge Now|wall");
	
	if (warnings > 3)
	  throw Miro::Exception("Killing program due to low batteries");
      }
      else {
	warnings = 0;
      }
    }
    /* ask the base for the next battery voltage */
    connection.batteryVoltage();

    return 0;
  }
};
