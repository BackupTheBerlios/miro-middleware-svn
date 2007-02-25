// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
