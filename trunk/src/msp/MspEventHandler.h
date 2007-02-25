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
#ifndef MspEventHandler_h
#define MspEventHandler_h

#include "abus/AbusEventHandler.h"

namespace Msp
{
  class Connection;
  class Consumer;

  class EventHandler : public Abus::EventHandler
  {
    typedef Abus::EventHandler Super;

  public:
    EventHandler(Connection& _msp, Miro::DevConsumer * _mspConsumer);
    virtual ~EventHandler();

  protected:
    virtual void disconnect    (const Abus::Message& message);
    virtual void linkApproveAck(const Abus::Message& message);

  private:
    Connection& msp;
  };
};
#endif




