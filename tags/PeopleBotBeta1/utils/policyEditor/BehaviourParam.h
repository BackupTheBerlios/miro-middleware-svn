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
  QString measure;		 // parameter measure
  QString defaultValue;          // parameter default value
  QString help;                  // parameter help text

public:
  BehaviourParam(const QString& _name,
		 const QString& _type,
		 const QString& _measuree,
		 const QString& _default = QString(),
		 const QString& _help = QString());

  const QString& getName() const;
  const QString& getType() const;
  const QString& getMeasure() const;
  const QString& getDefault() const;
  const QString& getHelp() const;
};

inline
BehaviourParam::BehaviourParam(const QString& _name, 
			       const QString& _type, 
			       const QString& _measure,
			       const QString& _default,
			       const QString& _help) : 
  name(_name),
  type(_type),
  measure(_measure),
  defaultValue(_default),
  help(_help)
{ }

inline
const QString& 
BehaviourParam::getName() const {
  return name; 
}

inline
const QString& 
BehaviourParam::getType() const {
  return type; 
}

inline
const QString&
BehaviourParam::getMeasure() const {
  return measure; 
}

inline
const QString&
BehaviourParam::getDefault() const {
  return defaultValue; 
}

inline
const QString&
BehaviourParam::getHelp() const {
  return help; 
}

#endif
