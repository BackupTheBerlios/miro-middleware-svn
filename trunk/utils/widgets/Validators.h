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
  
  typedef QIntValidator Super;
public:
  MyIntValidator(QObject * parent, const char *name = 0 );
  MyIntValidator(int bottom, int top,
		 QObject * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;

signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

//! Validator for double values.
class MyDoubleValidator : public QDoubleValidator
{
  Q_OBJECT
  
  typedef QDoubleValidator Super;
public:
  MyDoubleValidator(QObject * parent, const char *name = 0 );
  MyDoubleValidator(double bottom, double top, int decimals,
		    QObject * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;
    
signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

//! Validator for boolean values.
class MyBoolValidator : public QValidator
{
  Q_OBJECT
  
  typedef QValidator Super;
public:
  MyBoolValidator(QObject * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;
    
signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

#endif Validators_h
