// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Validators.h"

MyIntValidator::MyIntValidator(QObject * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

MyIntValidator::MyIntValidator(int bottom, int top,
			       QObject * parent, const char *name) :
  Super(bottom, top, parent, name),
  accepted_(true)
{}

QValidator::State 
MyIntValidator::validate( QString & input, int & pos) const
{
  QValidator::State s = Super::validate(input, pos);
  bool accept = input.isEmpty() || (s == Acceptable);
  if (s != Invalid &&
      accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyIntValidator *>(this)->acceptable(accept);
  }
  return s;
}

MyDoubleValidator::MyDoubleValidator(QObject * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

MyDoubleValidator::MyDoubleValidator(double bottom, double top, int decimals,
				     QObject * parent, const char *name) :
  Super(bottom, top, decimals, parent, name),
  accepted_(true)
{}

QValidator::State 
MyDoubleValidator::validate( QString & input, int & pos) const
{
  QValidator::State s = Super::validate(input, pos);
  bool accept = input.isEmpty() || (s == Acceptable);
  if (s != Invalid &&
      accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyDoubleValidator *>(this)->acceptable(accept);
  }

  return s;
}

MyBoolValidator::MyBoolValidator(QObject * parent, const char *name) :
  Super(parent, name),
  accepted_(true)
{}

QValidator::State 
MyBoolValidator::validate( QString & input, int &) const
{
  QValidator::State s = Invalid;

  if (input == "true" ||
      input == "false" ||
      input.length() == 0)
    s = Acceptable;
  else if (input.length() <= 5)
    s = Intermediate;
 
  bool accept = (s == Acceptable);
  if (s != Invalid &&
      accept != accepted_) {
    accepted_ = accept;
    emit const_cast<MyBoolValidator *>(this)->acceptable(accept);
  }

  return s;
}

