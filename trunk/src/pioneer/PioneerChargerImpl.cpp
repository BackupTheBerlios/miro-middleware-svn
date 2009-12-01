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
// $Id: PioneerChargerImpl.cpp 1812 2007-02-25 07:09:56Z hutz $
//
#include "pioneer/PioneerChargerImpl.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/Parameters.h"

#include "miro/Synch.h"
#include "miro/Exception.h"

#include "psos/PsosMessage.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Pioneer
{
  using std::cout;
  using std::cerr;
  using std::endl;


  PioneerChargerImpl::PioneerChargerImpl(Pioneer::Connection& _connection) throw(Miro::Exception) :
    connection(_connection),
    flags(0)
  {
    DBG(cout << "Constructing PioneerChargerImpl" << endl);
  }

  PioneerChargerImpl::~PioneerChargerImpl()
  {
    DBG(cout << "Destructing PioneerChargerImpl" << endl);
  }

  // 
  // IDL interface implementation

  void 
  PioneerChargerImpl::deployCharger() throw (Miro::EDevIO)
  {
    Psos::Message msg(68,(unsigned short)1);
    connection.writeMessage(msg);
  }

  void 
  PioneerChargerImpl::retractCharger() throw (Miro::EDevIO)
  {
    Psos::Message msg(68,(unsigned short)0);
    connection.writeMessage(msg);
  }
  unsigned short 
  PioneerChargerImpl::getChargerStatus() throw (Miro::EDevIO)
  {
    return (flags & 0x200);
  }

  void 
  PioneerChargerImpl::setFlags(const unsigned short _flags)
  {
	flags = _flags;
  }

};
