// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "FaulTtyMessage.h"

#include <stdio.h>
#include <stdlib.h>

namespace FaulTty
{
  Message::Message(char daten[256])        //short daten)
  {
    strcpy(buffer_ ,daten);
  }
  
  void Message::setPos(long posL, long posR)
  {
    lPos=posL;
    rPos=posR;
  }
};
