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
#ifndef Generator_h
#define Generator_h

#include "Class.h"

#include <vector>
#include <set>

class Generator
{
public:
  Generator();

  void addinclude(const QString& _include);
  void addInclude(const QString& _Include);

  void setBaseName(const QString& _baseName);
  void setHeaderExtension(const QString& _extensionName);
  void setNamespace(const QString& _namespace);

  void addClass(const Class& _class);

  void generateHeader(ostream& ostr) const;
  void generateSource(ostream& ostr) const;

protected:
  typedef std::set<QString> QStringSet;
  typedef std::vector<Class> ClassVector;

  QStringSet include_;
  QStringSet Include_;

  QString fileName_;
  QString extensionName_;
  QString namespace_;

  ClassVector class_;

  // static data
  static const unsigned long STEP = 2;
};

#endif
