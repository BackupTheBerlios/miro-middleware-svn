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
#ifndef CanEventHandler_h
#define CanEventHandler_h

#include "miro/DevEventHandler.h"

namespace Can
{
  // forward declarations
  class Message;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

  public:
    EventHandler(Miro::DevConsumer* _consumer);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select

  protected:
    Message * msg;
  };
};
#endif



