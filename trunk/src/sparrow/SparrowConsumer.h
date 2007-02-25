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
#ifndef SparrowConsumer_h
#define SparrowConsumer_h

#include "SparrowDevice.h"
#include "sparrow/Parameters.h"

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "miro/OdometryImpl.h"

// forward decleration
namespace Miro
{
  class RangeSensorImpl;
}

namespace Sparrow
{
  // forward declerations
  class Connection;
  class StallImpl;
  class ButtonsImpl;

  /**
   * Class for using the Sparrow robot
   */
  class Consumer : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;
    
  public:
    Consumer(Connection * _connection,
	     Miro::OdometryImpl * _pOdometry,
	     StallImpl * _pStall,
	     ButtonsImpl * _pButtons,
	     Miro::RangeSensorImpl * _pIR);
    Consumer();
    ~Consumer();

    virtual void handleMessage(const Miro::DevMessage * _message);
    virtual void registerInterfaces(Connection * _connection,
                                    Miro::OdometryImpl * _pOdometry,
                                    StallImpl * _pStall,
                                    ButtonsImpl * _pButtons,
                                    Miro::RangeSensorImpl * _pIR);

  protected:
    Connection * connection;
    Miro::OdometryImpl * pOdometry_;
    StallImpl * pStall_;
    ButtonsImpl * pButtons_;
    Miro::RangeSensorImpl * pIR_;

    Parameters const * params_;

    Miro::MotionStatusIDL status_;

  public:
    unsigned char   digital[8];
    unsigned short  analog[16];

    Miro::Mutex     motorAliveMutex;
    Miro::Condition motorAliveCond;
    Miro::Mutex     odoAliveMutex;
    Miro::Condition odoAliveCond;
    Miro::Mutex     portsAliveMutex;
    Miro::Condition portsAliveCond;
    Miro::Mutex     stallAliveMutex;
    Miro::Condition stallAliveCond;
    Miro::Mutex     kickerAliveMutex;
    Miro::Condition kickerAliveCond;
    Miro::Mutex     servoAliveMutex;
    Miro::Condition servoAliveCond;
    Miro::Mutex     dbgAliveMutex;
    Miro::Condition dbgAliveCond;

    Miro::Mutex     distanceLRMutex;
    Miro::Condition distanceLRCond;

    Miro::Mutex     digitalMutex;
    Miro::Condition digitalCond;

    Miro::Mutex     analogMutex;
    Miro::Condition analogCond;

    Miro::Mutex     irAliveMutex;
    Miro::Condition irAliveCond;

    short xPos_;
    short yPos_;
    double phi_;

    short xPrev_;
    short yPrev_;
    double phiPrev_;

    double x_;
    double y_;

    short distanceL;
    short distanceR;
  };
} // SparrowConsumer_h
#endif

