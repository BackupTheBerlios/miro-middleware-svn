
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldFactory.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
