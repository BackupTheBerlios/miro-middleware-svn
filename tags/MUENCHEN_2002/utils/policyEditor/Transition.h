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

#include <list>


//-----------------------------------------------------------------

class Transition
{
private:
  QString  message;         // message name
  QString  target;          // target pattern name

public:
  Transition() { }
  Transition(const QString& msg, const QString& tar);

  const QString& getTarget() const;
  const QString& getMessage() const;
};

typedef std::list<Transition> TransitionList;

inline
Transition::Transition(const QString& msg, const QString& tar) :
  message(msg),
  target(tar) 
{}

inline
const QString& 
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
#endif
