// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include <tao/corba.h>

#include "TimedBehaviourParameters.h"
#include "TimeHelper.h"
#include "KeyValueList.h"

#include <iostream>
#include <string>

namespace Miro
{
  TimedBehaviourParameters::TimedBehaviourParameters() :
    pace(0, 100000) // 10 Hz
  {}

  void
  TimedBehaviourParameters::operator <<= (const KeyValueList& _params)
  {
    QString str = _params.getValue("Pace");
    if (str.length() != 0)
      pace.msec(str.toInt());
  }

  void
  TimedBehaviourParameters::printToStream(std::ostream& ostr) const
  {
    Super::printToStream(ostr);
    ostr << "Pace (msec): " << pace << endl;
  }
};
