// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BehaviourDescription_hh
#define BehaviourDescription_hh

#include <qstring.h>

//-----------------------------------------------------------------

class BehaviourDescription
{
private:
  QString name;                  // behaviour name

public:
  BehaviourDescription(const QString& Name);

  const QString& getName() const;
};

inline
BehaviourDescription::BehaviourDescription(const QString& _name) : 
  name(_name)
{ }

inline
const QString& 
BehaviourDescription::getName() const 
{
  return name; 
}
#endif
