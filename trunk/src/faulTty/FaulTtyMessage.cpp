// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////


#include "FaulTtyMessage.h"

#include <iostream>
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
