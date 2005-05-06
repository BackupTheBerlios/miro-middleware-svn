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


#include "PCanMessage.h"

#include <iostream>

namespace Can
{


  PCanMessage::PCanMessage()
  {

      messagep_ = new pcanmsg;

  }

  PCanMessage::~PCanMessage() {
    delete messagep_;
  }
  
  void PCanMessage::setBuffer(int pos, const char * buffer, int length){
      memcpy(&(messagep_->msg.data[pos]), buffer , length);
  }
}
