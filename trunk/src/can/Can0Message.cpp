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


#include "Can0Message.h"

#include <iostream>

namespace Can
{


  Can0Message::Can0Message()
  {
     message_ = new canmsg;

  }

  void Can0Message::setBuffer(int pos, const char * buffer, int length){
      memcpy(&(message_->d[pos]), buffer , length);
  }
};
