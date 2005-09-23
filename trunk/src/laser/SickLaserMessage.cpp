// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SickLaserMessage.h"

#include <iostream>

namespace Miro
{
  void 
  LaserMessage::PrintDataOn(std::ostream &_ostr) const 
  {
    for (long i=0; i<(datalength()); ++i) 
      _ostr << data()[i];
    _ostr << std::endl;
    _ostr << "Status : " << int(status()) << std::endl;
  }

  void 
  LaserMessage::PrintDumpOn(std::ostream &_ostr) const 
  {
    for (long i=0; i<length()+6; ++i)
      _ostr << short(bufferAddress()[i]) << " ";
    _ostr << "}" << std::endl;
  }


  std::ostream &
  operator<<(std::ostream &_ostr, const LaserMessage &lm) 
  {
    _ostr << "[address:" << int(lm.address()) 
	  << " length:" << lm.length()
	  << " cmd:" << int(lm.cmd())
	  << " status:" << int(lm.status())
	  << " crc:" << lm.crc() << "]{";

    for (long i=0; i<lm.length(); ++i)
      _ostr << short(lm.data()[i]) << " ";
    _ostr << "}" << std::endl;

    return _ostr;
  }
}
