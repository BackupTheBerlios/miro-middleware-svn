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
#ifndef ParameterDialog_h
#define ParameterDialog_h

#include "../../src/params/Class.h"

#include <qdialog.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qmap.h>

#include <vector>

// forward declarations
class PolicyConfigClass;
class QLineEdit;

class MyIntValidator : public QIntValidator
{
  Q_OBJECT
  
  typedef QIntValidator Super;
public:
  MyIntValidator(QWidget * parent, const char *name = 0 );
  MyIntValidator(int bottom, int top,
		 QWidget * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;

signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

class MyDoubleValidator : public QDoubleValidator
{
  Q_OBJECT
  
  typedef QDoubleValidator Super;
public:
  MyDoubleValidator(QWidget * parent, const char *name = 0 );
  MyDoubleValidator(double bottom, double top, int decimals,
		    QWidget * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;
    
signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

class MyBoolValidator : public QValidator
{
  Q_OBJECT
  
  typedef QValidator Super;
public:
  MyBoolValidator(QWidget * parent, const char *name = 0 );
		   
  QValidator::State validate( QString &, int & ) const;
    
signals:
  void acceptable(bool);

protected:
  mutable bool accepted_;
};

/**
 * This class shows a dialog for the behaviour parameters.
 */
class ParameterDialog : public QDialog
{
  Q_OBJECT

  typedef QDialog Super;
  typedef std::vector<QLineEdit *> LineEditVector;

public:
  ParameterDialog(const QString& _behaviourName, 
		  const QMap<QString, QString>& _stringMap,
		  QWidget * _parent);

  bool modified();

  QMap<QString, QString> result() const;

protected slots:
  void accept(bool _accept);

signals:
  void okay(bool);

protected:
  PolicyConfigClass * config_;
  const Class * class_;
  Class::ParameterSet params_;
  bool modified_;
  int accept_;
  
private:
  LineEditVector lineEdits_;
};

inline
bool
ParameterDialog::modified() {
  return modified_;
}

#endif
