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
#ifndef BehaviourParam_hh
#define BehaviourParam_hh

#include <qstring.h>

//-----------------------------------------------------------------

class BehaviourParam
{
private:
  QString name;                  // parameter name
  QString type;			 // parameter type
  QString measure;		// parameter measure

public:
  BehaviourParam(const QString& Name, const QString& Type, const QString& Measure);

  const QString& getName() const;
  const QString& getType() const;
  const QString& getMeasure() const;
};

inline
BehaviourParam::BehaviourParam(const QString& Name, const QString& Type, const QString& Measure) : 
  name(Name),
  type(Type),
  measure(Measure)  
{ }

inline
const QString& 
BehaviourParam::getName() const 
{
  return name; 
}

inline
const QString& 
BehaviourParam::getType() const 
{
  return type; 
}

inline
const QString&
BehaviourParam::getMeasure() const 
{
  return measure; 
}

#endif
