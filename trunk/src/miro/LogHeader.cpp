// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "LogHeader.h"
#include "Exception.h"

#include <string>
#include <sstream>

namespace Miro
{
  LogHeader::LogHeader(READ) throw (EFileType, EVersion)
  {
    if (id != PROTOCOL_ID) {
      char idString[5] = "    ";
      *(reinterpret_cast<unsigned long *>(idString)) = id;
      throw EFileType(std::string("Wrong protocol ID for a Miro Log File (should be MLOG):") + idString);
    }
    
    if (version > MAX_VERSION) {
      std::ostringstream o;
      o << version;
      throw EVersion("Unsupported protocol version: " + o.str());
    }
  }
}  
