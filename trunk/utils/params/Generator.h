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
#include <map>
#include <set>

class Generator
{
public:
  typedef std::multimap<QString, Class> GroupMap;

  Generator();

  void addinclude(const QString& _include);
  void addInclude(const QString& _Include);
  void addSrcInclude(const QString& _Include);

  void addForwardDeclaration(const QString& _decl);
  void addLocalForwardDeclaration(const QString& _decl);

  void setBaseName(const QString& _baseName);
  void setHeaderExtension(const QString& _extensionName);
  void addNamespace(const QString& _namespace);

  void addClass(const QString& _group, const Class& _class);

  void generateHeader(ostream& ostr) const;
  void generateSource(ostream& ostr) const;

  void getGroupedClasses(const QString& _group, 
			 GroupMap::const_iterator& _first, GroupMap::const_iterator& _last) const;
  const Class * getClass(const QString& _name) const;

protected:
  typedef std::set<QString> QStringSet;
  typedef std::vector<QString> QStringVector;
  typedef std::vector<Class> ClassVector;

  QStringSet include_;
  QStringSet Include_;
  QStringSet srcInclude_;

  QStringSet forwardDeclaration_;
  QStringSet localForwardDeclaration_;

  QString fileName_;
  QString extensionName_;
  QStringVector namespace_;

  ClassVector class_;
  GroupMap groups_;

  // static data
  static const unsigned long STEP = 2;
};

#endif
