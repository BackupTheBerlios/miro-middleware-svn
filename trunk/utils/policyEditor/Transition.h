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
#ifndef Transition_hh
#define Transition_hh

#include <qstring.h>

#include <vector>

// forward declarations
class PatternXML;

//-----------------------------------------------------------------

class Transition
{
private:
  QString  message;         // message name
  PatternXML *  target;          // target pattern name

public:
  Transition() { }
  Transition(const QString& msg, PatternXML * tar);

  PatternXML * getTarget() const;
  const QString& getMessage() const;

  bool operator < (Transition const& _rhs) const;
};

typedef std::vector<Transition> TransitionVector;

inline
Transition::Transition(const QString& msg, PatternXML * tar) :
  message(msg),
  target(tar) 
{}

inline
PatternXML * 
Transition::getTarget() const
{
  return target; 
}

inline
const QString&
Transition::getMessage() const 
{
  return message; 
}

inline
bool
Transition::operator < (Transition const& _rhs) const {
  return message < _rhs.message;
}
#endif
