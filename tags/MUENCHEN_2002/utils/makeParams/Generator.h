// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Generator_h
#define Generator_h

#include "Parameter.h"

#include <vector>
#include <set>
#include <map>

class Generator
{
public:
  void addinclude(const QString& _include);
  void addInclude(const QString& _Include);

  void setFileName(const QString& _fileName);
  void setNamespace(const QString& _namespace);
  void setName(const QString& _name);
  void setParent(const QString& _parent);

  void addStatic(const QString& _type, const QString& _name);
  void addParameter(const Parameter& _parameter);
  void addToConstructor(const QString& _ctor);

  void generateHeader();
  void generateSource();

  const QString& name() const;

protected:
  typedef std::pair<QString, QString> QStringPair;
  typedef std::vector<QStringPair> QStringPairVector;
  typedef std::set<QString> QStringSet;
  typedef std::set<Parameter> ParameterSet;
  typedef std::vector<QString> QStringVector;

  QStringSet include_;
  QStringSet Include_;

  QStringPairVector staticData_;

  QString fileName_;
  QString namespace_;
  QString name_;
  QString parent_;

  QStringVector ctor_;
  ParameterSet parameter_;
};

#endif
