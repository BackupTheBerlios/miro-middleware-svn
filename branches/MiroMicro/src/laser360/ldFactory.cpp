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
#include "ldFactory.h"

using namespace std;

namespace ldoem_
{

  //---------------------------------------------------------------------------
  BaseTelegram * TgmFactory::getTGM( uint16 identifier ) throw( TgmFactoryException )
  {
    if ( factory_.find( identifier ) != factory_.end() )
      return factory_[identifier];
    else
    {
      stringstream message_stream;
      message_stream << "TgmFactory::getTGM : Unknown Telegram! type<" << ( uint16 )identifier << ">";
      throw TgmFactoryException( message_stream.str() );
    }
    return 0;
  }

  //---------------------------------------------------------------------------
  void TgmFactory::showRegisteredTelegrams()
  {
    if ( factory_.size() == 0 )
    {
      cerr << "TgmFactory::showRegisteredTelegrams : Factory is empty..." << endl;
      return;
    }
    map < uint16, BaseTelegram * >::iterator it;
    for ( it = factory_.begin(); it != factory_.end(); it++ )
    {
      cerr << " Telegram Type <" << hex << ( uint16 )( * it ).first << ">, Address <" << ( * it ).second
         << ">" << dec << endl;
    }
  }
}; // end namespace ldoem_
