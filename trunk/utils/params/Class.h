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
  typedef std::set<Parameter> ParameterSet;

  Class();

  void setFinal(bool _final);
  bool isFinal() const;
  void setDummy(bool _dummy);
  bool isDummy() const;
  void setExtern(bool _extern);
  bool isExtern() const;
  void setName(const QString& _name);
  const QString& name() const;
  void setParent(const QString& _parent);
  const QString& parent() const;

  void addStatic(const QString& _type, const QString& _name);
  void addParameter(const Parameter& _parameter);
  void addToConstructor(const QString& _ctor);

  void generateHeader(std::ostream& ostr, unsigned long _indent) const;
  void generateSource(std::ostream& ostr, unsigned long _indent) const;

  const ParameterSet& parameterSet() const;

protected:
  void generateQDomOutOperator(std::ostream& ostr, 
			       const QString& classPrefix, unsigned long indent) const;
  void generateQDomInOperator(std::ostream& ostr, 
			      const QString& classPrefix, unsigned long indent) const;

  typedef std::pair<QString, QString> QStringPair;
  typedef std::vector<QStringPair> QStringPairVector;
  typedef std::vector<QString> QStringVector;

  QStringPairVector staticData_;

  QString name_;
  QString parent_;

  bool final_;
  bool dummy_;
  bool extern_;

  QStringVector ctor_;
  ParameterSet parameter_;

  // static data
  static const unsigned long STEP = 2;
};

inline
const Class::ParameterSet&
Class::parameterSet() const {
  return parameter_;
}

inline
void 
Class::setFinal(bool _final) {
  final_ = _final;
}

inline
bool 
Class::isFinal() const {
  return final_;
}

inline
void 
Class::setDummy(bool _dummy) {
  dummy_ = _dummy;
}

inline
bool 
Class::isDummy() const {
  return dummy_;
}

inline
void 
Class::setExtern(bool _extern) {
  extern_ = _extern;
}

inline
bool 
Class::isExtern() const {
  return extern_;
}

inline
void
Class::setName(const QString& _name) {
  name_ = _name;
}

inline
const QString&
Class::name() const {
  return name_;
}

inline
void
Class::setParent(const QString& _parent) {
  parent_ = _parent;
}

inline
const QString&
Class::parent() const {
  return parent_;
}

#endif
