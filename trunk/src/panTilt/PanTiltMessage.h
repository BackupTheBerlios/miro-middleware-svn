// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PanTiltMessage_h
#define PanTiltMessage_h

#include "PanTiltOpcodes.h"

#include "miro/DevMessage.h"
#include "miro/Exception.h"

#include <strstream>

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
      std::string s(data, offset, data.length()-offset);
      std::istrstream sstr(s.c_str(),s.length());
      long result;
      sstr >> result;
      return result;
    }
  };

  inline 
  std::ostream& operator << (std::ostream &ostr, const Message &lm) {
    ostr << lm.getMessage() << endl;

    return ostr;
  }
};
#endif






