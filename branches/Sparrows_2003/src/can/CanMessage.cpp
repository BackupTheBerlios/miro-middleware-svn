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


#include "CanMessage.h"

#include <iostream>

namespace Can
{
  drivertype Message::driver = UNINITIALIZED;

  Message::Message() {
     
     if(driver == OLD){
        message_ = new canmsg;
     }
     else if(driver == PCAN){
        messagep_ = new pcanmsg;
     }
     else{
        message_ = new canmsg;
	messagep_ = new pcanmsg;
     }



  }

  std::ostream& operator<< (std::ostream& ostr, const Message& rhs)
  {
    ostr.setf(ios::hex, ios::basefield);
    ostr << "id=" << rhs.id() << "  msg= 0x";
    for (int i = 0; i < rhs.length(); ++i) {
      ostr << (int)(rhs.byteData(i)) << " ";
    }
    ostr.setf(ios::dec, ios::basefield);
    return ostr;
  }

  void Message::setBuffer(int pos, const char * buffer, int length){
       if(driver==OLD)
		memcpy(&(message_->d[pos]), buffer , length);
       else
		memcpy(&(messagep_->Msg.DATA[pos]), buffer , length);

       
  }


};
