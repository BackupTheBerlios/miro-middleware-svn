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
#ifndef Miro_CFG_Generator_h
#define Miro_CFG_Generator_h

#include "Type.h"

#include <vector>
#include <map>
#include <set>

namespace Miro
{
  namespace CFG
  {
    class Generator;
    std::ostream& operator << (std::ostream& _ostr, Generator const& _rhs);

    class Generator
    {
    public:
      typedef std::multimap<QString, Type> GroupMap;
      typedef std::vector<QString> QStringVector;

      Generator();

      void addinclude(const QString& _include);
      void addInclude(const QString& _Include);
      void addSrcInclude(const QString& _Include);

      void addForwardDeclaration(const QString& _decl);
      void addLocalForwardDeclaration(const QString& _decl);

      void setBaseName(const QString& _baseName);
      void setHeaderExtension(const QString& _extensionName);
      void addNamespace(const QString& _namespace);
      void clearNamespace();

      void addType(const QString& _group, const Type& _class);

      void generateHeader(std::ostream& ostr) const;
      void generateSource(std::ostream& ostr) const;

      QStringVector groups() const;
      void getGroupedTypes(const QString& _group, 
			   GroupMap::const_iterator& _first, 
			   GroupMap::const_iterator& _last) const;
      const Type * getType(const QString& _name) const;


      ParameterVector getFullParameterSet(Type const& _type) const;
      bool isDerivedType(Type const& _type, Type const& _ancestor) const;
      QStringVector getDescendants(Type const& _type) const;

    protected:
      typedef std::set<QString> QStringSet;
      typedef std::vector<Type> TypeVector;

      QStringSet include_;
      QStringSet Include_;
      QStringSet srcInclude_;

      QStringSet forwardDeclaration_;
      QStringSet localForwardDeclaration_;

      QString fileName_;
      QString extensionName_;
      QStringVector namespace_;

      TypeVector type_;
      GroupMap groups_;

      // static data
      static const unsigned long STEP = 2;

      friend std::ostream& operator << (std::ostream& _ostr, Generator const& _rhs);
    };
  }
}
#endif // Miro_CFG_Generator_h
