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
#ifndef Validators_h
#define Validators_h

#include <qvalidator.h>

//! Validator for intager values.
class MyIntValidator : public QIntValidator
{
  Q_OBJECT
  //! Base class type.
  typedef QIntValidator Super;

public:
  //! Inherited method.
  MyIntValidator(QObject * parent, const char *name = 0);
  //! Inherited method.
  MyIntValidator(int bottom, int top,
		 QObject * parent, const char *name = 0);
  //! Inherited method.
  QValidator::State validate(QString &, int &) const;

signals:
  //! Signals, if the input is valid.
  void acceptable(bool);

protected:
  //! Remember, last state.
  mutable bool accepted_;
};

//! Validator for double values.
class MyDoubleValidator : public QDoubleValidator
{
  Q_OBJECT
  //! Base class type.
  typedef QDoubleValidator Super;

public:
  //! Inherited method.
  MyDoubleValidator(QObject * parent, const char *name = 0);
  //! Inherited method.
  MyDoubleValidator(double bottom, double top, int decimals,
		    QObject * parent, const char *name = 0);
  //! Inherited method.
  QValidator::State validate(QString &, int &) const;
    
signals:
  //! Signals, if the input is valid.
  void acceptable(bool);

protected:
  //! Remember, last state.
  mutable bool accepted_;
};

//! Validator for boolean values.
class MyBoolValidator : public QValidator
{
  Q_OBJECT
  //! Base class type.
  typedef QValidator Super;

public:
  //! Inherited method.
  MyBoolValidator(QObject * parent, const char *name = 0);
  //! Inherited method.
  QValidator::State validate(QString &, int &) const;
    
signals:
  //! Signals, if the input is valid.
  void acceptable(bool);

protected:
  //! Remember, last state.
  mutable bool accepted_;
};

#endif // Validators_h

