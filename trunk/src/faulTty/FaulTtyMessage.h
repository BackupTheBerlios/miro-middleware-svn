// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulTtyMessage_h
#define FaulTtyMessage_h

#include "miro/DevMessage.h"

//#include "FaulTtyDevice.h"

#include <cmath>

namespace FaulTty
{
  // forward declarations
  class Message;

  class Message : public Miro::DevMessage
  {
  public:
    Message() {} // default get message
    Message(char daten[256]); //short daten);
    void setPos(long posL, long posR);
    char buffer_[256];
    long lPos;
    long rPos;
  }; 
};
#endif 
