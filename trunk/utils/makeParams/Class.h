// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Class_h
#define Class_h

#include "Parameter.h"

#include <vector>
#include <set>
#include <map>

class Class
{
public:
  void setName(const QString& _name);
  void setParent(const QString& _parent);

  void addStatic(const QString& _type, const QString& _name);
  void addParameter(const Parameter& _parameter);
  void addToConstructor(const QString& _ctor);

  void generateHeader(ostream& ostr, unsigned long _indent) const;
  void generateSource(ostream& ostr, unsigned long _indent) const;

  const QString& name() const;

protected:
  typedef std::pair<QString, QString> QStringPair;
  typedef std::vector<QStringPair> QStringPairVector;
  typedef std::set<Parameter> ParameterSet;
  typedef std::vector<QString> QStringVector;

  QStringPairVector staticData_;

  QString name_;
  QString parent_;

  QStringVector ctor_;
  ParameterSet parameter_;

  // static data
  static const unsigned long STEP = 2;
};

#endif
