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


#include "KeyValueList.h"

namespace Miro
{
  void
  KeyValueList::addPair(const QString& _name, const QString& _value) 
  {
    parameters_[_name] = _value;	
  }

  QString
  KeyValueList::getValue(const QString& _name) const
  {
    KeyValueListMap::const_iterator i = parameters_.find(_name);
    return (i != parameters_.end())? i->second : QString();
  }

  void 
  KeyValueList::printToConsole() 
  {
    for(KeyValueListMap::const_iterator i = parameters_.begin(); i != parameters_.end(); ++i) {
      cout << i->first << "=\"" << i->second << "\"" << endl;
    }
  }
};
