
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldFactory.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
