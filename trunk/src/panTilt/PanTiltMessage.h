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
#ifndef PanTiltMessage_h
#define PanTiltMessage_h

#include "PanTiltOpcodes.h"

#include "miro/DevMessage.h"
#include "miro/Exception.h"

#include <sstream>

namespace DpPanTilt 
{
  /**
   * represents a message block sent to and received from the 
   * panTilt unit. 
   *
   */
  class Message : public Miro::DevMessage
  {
    std::string data;
    bool error;

  public:
    /**
     * constructs an empty Message object
     */
    Message()                     { error = false; }

    Message( const std::string &comm ) : data(comm) { error = false;}

    const std::string &getMessage()          const { return data; }

    void addChar( char c )               { data += c; }
    void addString( const std::string &s  )   { data += s; }
    void addLong( long l )               { 
      char buffer[80];
      sprintf(buffer,"%ld",l);
      data += buffer;
      // this is how it should be but seems not to work very corectly
      //       strstream s; 
      //       s << l; 
      //       data += s.str(); 
    }
    void resetMessage()                  { data = ""; error = false; }
    void setErrorState()                 { error = true; }
    bool getErrorState()                 const { return error; }
    long getLong( int offset ) const { 
      std::string s(data, offset, data.length() - offset);
      std::istringstream sstr(s);
      long result;
      sstr >> result;
      return result;
    }
  };

  inline 
  std::ostream& operator << (std::ostream &ostr, const Message &lm) {
    ostr << lm.getMessage() << std::endl;

    return ostr;
  }
};
#endif






