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
