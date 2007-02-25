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
#include "ldInOutHandler.h"
#include <cstring>
#include <cstdio>

namespace ldoem_
{
  //---------------------------------------------------------------------------
  // default empty string for the functor
  const char * const InOutHandler::emptyString = "";

  // --------------------------------------------------
  // InOutHandler
  // --------------------------------------------------

  //---------------------------------------------------------------------------
  // default constructor
  InOutHandler::InOutHandler() : Object(), level( 0 ), statusString( 0 )
  {
  }

  //---------------------------------------------------------------------------
  // copy constructor
  InOutHandler::InOutHandler( const InOutHandler & other ) : Object(), statusString( 0 )
  {
    copy( other );
  }

  //---------------------------------------------------------------------------
  // destructor
  InOutHandler::~InOutHandler()
  {
    delete[] statusString;
    statusString = 0;
  }

  //---------------------------------------------------------------------------
  // returns the name of this type
  const char * InOutHandler::getTypeName() const
  {
    return "InOutHandler";
  }

  //---------------------------------------------------------------------------
  // copy member
  InOutHandler & InOutHandler::copy( const InOutHandler & other )
  {
    level = other.level;
    return ( * this );
  }

  //---------------------------------------------------------------------------
  const int & InOutHandler::getLevel() const
  {
    return level;
  }

  //---------------------------------------------------------------------------
  void InOutHandler::resetLevel( const int & theLevel )
  {
    level = theLevel;
  }

  //---------------------------------------------------------------------------
  bool InOutHandler::writeBegin()
  {
    level++;
    return true;
  }

  //---------------------------------------------------------------------------
  bool InOutHandler::writeEnd()
  {
    level--;
    return ( level >= 0 );
  }

  //---------------------------------------------------------------------------
  bool InOutHandler::readBegin()
  {
    level++;
    return true;
  }

  //---------------------------------------------------------------------------
  bool InOutHandler::readEnd()
  {
    level--;
    return ( level >= 0 );
  }

  //---------------------------------------------------------------------------
  /// @name write members for symbol data pairs
  bool InOutHandler::write( const std::string & name, const std::string & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const char * data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const double & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const float & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const int & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const unsigned int & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const char & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const signed char & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const unsigned char & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const bool & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const long & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const unsigned long & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const short & data )
  {
    return ldoem_::write( * this, name, data );
  };

  bool InOutHandler::write( const std::string & name, const unsigned short & data )
  {
    return ldoem_::write( * this, name, data );
  };

  //---------------------------------------------------------------------------
  /// @name write member for standard types
  bool write( InOutHandler & handler, const std::string & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const char * data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const double & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const float & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const int & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const unsigned int & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const char & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const signed char & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const unsigned char & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const bool & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const long & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const unsigned long & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const short & data )
  {
    return handler.write( data );
  }

  bool write( InOutHandler & handler, const unsigned short & data )
  {
    return handler.write( data );
  }

  //---------------------------------------------------------------------------
  /// @name read members for symbol data pairs

  bool InOutHandler::read( const std::string & name, std::string & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, double & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, float & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, int & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, unsigned int & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, char & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, signed char & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, unsigned char & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, bool & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, long & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, unsigned long & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, short & data )
  {
    return ldoem_::read( * this, name, data );
  };

  bool InOutHandler::read( const std::string & name, unsigned short & data )
  {
    return ldoem_::read( * this, name, data );
  };

  //---------------------------------------------------------------------------
  /// @name read member for standard types

  bool read( InOutHandler & handler, std::string & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, double & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, float & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, int & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, unsigned int & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, char & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, signed char & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, unsigned char & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, bool & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, long & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, unsigned long & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, short & data )
  {
    return handler.read( data );
  }

  bool read( InOutHandler & handler, unsigned short & data )
  {
    return handler.read( data );
  }

  //---------------------------------------------------------------------------
  /// return the last message set with setStatusString().  This will
  /// never return 0.  If no status-string has been set yet an empty string
  /// (pointer to a string with only the char(0)) will be returned.
  const char * InOutHandler::getStatusString() const
  {
    if ( isNull( statusString ) )
    {
      return emptyString;
    }
    else
    {
      return statusString;
    }
  }

  //---------------------------------------------------------------------------
  /// set a status string.
  /// @param msg the const string to be reported next time by
  /// getStatusString()
  /// This message will be usually set within the apply methods to indicate
  /// an error cause.
  void InOutHandler::setStatusString( const char * msg ) const
  {
    delete[] statusString;
    statusString = 0;

    statusString = new char[strlen( msg ) + 1];
    strcpy( statusString, msg );
  }

  //---------------------------------------------------------------------------
  void InOutHandler::setStatusString( const std::string & msg ) const
  {
    setStatusString( msg.c_str() );
  }

  //---------------------------------------------------------------------------
  void InOutHandler::appendStatusString( const char * msg ) const
  {
    char * oldStatus = statusString;

    if ( isNull( oldStatus ) )
    {
      setStatusString( msg );
      return;
    }

    statusString = 0;
    statusString = new char[strlen( oldStatus ) + strlen( msg ) + 1];
    strcpy( statusString, oldStatus );
    strcat( statusString, msg );
    delete[] oldStatus;
    oldStatus = 0;
  }

  //---------------------------------------------------------------------------
  void InOutHandler::appendStatusString( const std::string & msg ) const
  {
    appendStatusString( msg.c_str() );
  }

  //---------------------------------------------------------------------------
  void InOutHandler::appendStatusString( const int & msg ) const
  {
    char * oldStatus = statusString;
    char * buf = new char[50];
    sprintf( buf, "%i", msg );
    statusString = 0;
    statusString = new char[strlen( oldStatus ) + strlen( buf ) + 1];
    strcpy( statusString, oldStatus );
    strcat( statusString, buf );
    delete[] oldStatus;
    oldStatus = 0;
    delete[] buf;
  }

  //---------------------------------------------------------------------------
  void InOutHandler::appendStatusString( const double & msg ) const
  {
    char * oldStatus = statusString;
    char * buf = new char[50];
    sprintf( buf, "%g", msg );
    statusString = 0;
    statusString = new char[strlen( oldStatus ) + strlen( buf ) + 1];
    strcpy( statusString, oldStatus );
    strcat( statusString, buf );
    delete[] oldStatus;
    oldStatus = 0;
    delete[] buf;
  }

  //---------------------------------------------------------------------------
  void InOutHandler::appendContextStatus() const
  {
  }

}
