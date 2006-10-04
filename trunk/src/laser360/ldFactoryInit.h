
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldFactoryInit.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LD_FACTORY_INIT_H_
  #define _LD_FACTORY_INIT_H_

  #include "ldFactory.h"

namespace ldoem_
{


  std::map < uint16, BaseTelegram * > TgmFactory::factory_;

  //---------------------------------------------------------------------------
  /// Initialisation of the Factory with the GSD Components
  class TgmFactoryInitializer
  {
    static TgmFactoryInitializer ti_;

    TgmFactoryInitializer()
    {
      TgmFactory::factory_[GET_IDENTIFICATION] = new GetIndification();
      TgmFactory::factory_[GET_STATUS] = new GetStatus();
      TgmFactory::factory_[GET_SIGNAL] = new GetSignal();
      TgmFactory::factory_[SET_SIGNAL] = new SetSignal();
      TgmFactory::factory_[GET_CONFIG] = new GetConfig();
      TgmFactory::factory_[SET_TIME_ABS] = new SetTimeAbsolute();
      TgmFactory::factory_[SET_TIME_REL] = new SetTimeRelative();
      TgmFactory::factory_[GET_SYNC_CLOCK] = new GetSyncClock();
      TgmFactory::factory_[SET_CONFIG] = new SetConfig();
      TgmFactory::factory_[SET_FUNCTION] = new SetFunction();
      TgmFactory::factory_[GET_FUNCTION] = new GetFunction();
      TgmFactory::factory_[GET_PROFILE] = new GetProfile();
      TgmFactory::factory_[CANCEL_PROFILE] = new CancelProfile();
      TgmFactory::factory_[DO_RESET] = new DoReset();
      TgmFactory::factory_[TRANSE_IDLE] = new TranseIdle();
      TgmFactory::factory_[TRANSE_ROTATE] = new TranseRotate();
      TgmFactory::factory_[TRANSE_MEASURE] = new TranseMeasure();

  #ifdef DEBUG_MESSAGES
      TgmFactory::showRegisteredTelegrams();
  #endif
    }

  };

}; // end namespace ldoem_

#endif // _LD_FACTORY_INIT_H_
