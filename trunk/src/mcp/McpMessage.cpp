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


#include "McpMessage.h"

#include <iostream>

namespace Mcp
{
  inline
  std::ostream& operator << (std::ostream& ostr, const Message& message) {
    ostr << "size: " << (int) message.size() 
	 << "  opcode: 0x" << hex;
    ostr.width(2);
    ostr.fill('0');
    ostr << (int) message.opcode() << endl;

    for(int i = 0; i <= message.size(); ++i) {
      ostr.width(2);
      ostr.fill('0');
      ostr << (int) message.buffer()[i] << " ";
    }
    return ostr << dec;
  } 
};
