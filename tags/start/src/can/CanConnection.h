// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef CanConnection_h
#define CanConnection_h

// A general object oriented access class "CanConnection" for the CAN-bus
// interface.
//

#include "CanMessage.h"

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/DevConnection.h"

namespace Can
{
  // forward declerations
  class Parameters;

  class Connection : public Miro::DevConnection
  {
    typedef Miro::DevConnection Super;

  public:
    Connection(ACE_Reactor* _reactor, 
	       Miro::DevEventHandler* _eventHandler,
	       const Parameters& _parameters);

    void init() const;

    /** write up to 8 bytes to a 29bits long id */
    void write(const Message& message);

    ACE_Time_Value lastWrite;
    static ACE_Time_Value canTimeOut;

  protected:
    Miro::Mutex writeMutex;
  };

};
#endif
