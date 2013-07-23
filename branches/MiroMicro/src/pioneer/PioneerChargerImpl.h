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
// $Id: PioneerChargerImpl.h 1812 2007-02-25 07:09:56Z hutz $
//
#ifndef PioneerGripperImpl_hh
#define PioneerGripperImpl_hh

#include "idl/ChargerS.h"

#include "pioneer/PioneerConnection.h"


namespace Pioneer
{
  class Exception;

  class PioneerChargerImpl :  public virtual POA_Miro::Charger
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    PioneerChargerImpl(Pioneer::Connection& _connection) throw(Miro::Exception);
    virtual ~PioneerChargerImpl();

    virtual void deployCharger() throw (Miro::EDevIO);
    virtual void retractCharger() throw (Miro::EDevIO);
    virtual unsigned short getChargerStatus() throw (Miro::EDevIO);

    void setFlags(const unsigned short _flags);

  protected:
    Pioneer::Connection & connection; // encapsulating communication to hardware
    unsigned short flags; // status flags of Pioneer Robot
  };

};

#endif




