// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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
#ifndef _LD_FACTORY_H_
  #define _LD_FACTORY_H_

  #include <iostream>
  #include <map>
  #include "ldBaseTelegram.h"
  #include "ldUserTypes.h"
  #include "ldScannerTelegram.h"
  #include "ldTelegramCodes.h"
  #include "ldFactoryException.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  /// Telegram Factory class
  class TgmFactory
  {
  public:

    //---------------------------------------------------------------------------
    /// TgmFactoryInitializer
    ///
    /// Used for this kind of initialisation
    friend class TgmFactoryInitializer;

    //---------------------------------------------------------------------------
    /// Virtual Destructor
    virtual ~TgmFactory()
    {
    };

    //---------------------------------------------------------------------------
    /// getTGM
    ///
    /// Returns a pointer to the Object saved in factory
    ///
    /// @param identifier Objectidentifier
    /// @return Pointer to Object stored in Factory
    static BaseTelegram * getTGM( uint16 identifier ) throw( TgmFactoryException );

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------
  private:

    //---------------------------------------------------------------------------
    /// factory map
    static std::map < uint16, BaseTelegram * > factory_;

    //---------------------------------------------------------------------------
    // private methods
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    /// showRegisteredTelegrams
    ///
    /// Print all Telegram-Objects registered in factory
    static void showRegisteredTelegrams();
  };

}; // end namespace ldoem_

#endif // _LD_FACTORY_H_
