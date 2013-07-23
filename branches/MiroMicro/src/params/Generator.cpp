// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Generator.h"

#include <qfileinfo.h>

#include <iostream>
#include <algorithm>

#ifndef WIN32
#include <sys/time.h>
#else
#include <qdatetime.h>
#endif // WIN32

#include "qt_compatibility.h"


namespace
{
  QString head(QString("// -*- c++ -*- ///////////////////////////////////////////////////////////////\n") +
               QString("// This file was auto-generated by MakeParameters\n") +
               QString("//////////////////////////////////////////////////////////////////////////////"));
  QString spaces("                                                        ");
};

namespace Miro
{
  namespace CFG {
    Generator::Generator()
    {
      addInclude("iostream");
      addInclude("qdom.h");

    }

    void
    Generator::addinclude(const QString& _include)
    {
      include_.insert(_include);
    }

    void
    Generator::addInclude(const QString& _Include)
    {
      Include_.insert(_Include);
    }

    void
    Generator::addSrcInclude(const QString& _Include)
    {
      srcInclude_.insert(_Include);
    }

    void
    Generator::addForwardDeclaration(const QString& _decl)
    {
      forwardDeclaration_.insert(_decl);
    }

    void
    Generator::addLocalForwardDeclaration(const QString& _decl)
    {
      localForwardDeclaration_.insert(_decl);
    }

    void
    Generator::addType(const QString& _group, const Type& _type)
    {
      type_.push_back(_type);
      QString nameSpace;
      QStringVector::const_iterator first, last = namespace_.end();
      for (first = namespace_.begin(); first != last; ++first)
        nameSpace += (*first) + "::";
      type_.back().setNameSpace(nameSpace);
      groups_.insert(std::make_pair(_group, type_.back()));
    }

    void
    Generator::setBaseName(const QString& _fileName)
    {
      fileName_ = _fileName;
    }

    void
    Generator::setHeaderExtension(const QString& _extensionName)
    {
      extensionName_ = _extensionName;
    }

    void
    Generator::addNamespace(const QString& _namespace)
    {
      namespace_.push_back(_namespace);
    }

    void
    Generator::generateHeader(std::ostream& ostr, const QString& exportDirective) const
    {
      if (fileName_.isEmpty())
        throw QString("No file name specified.");

#ifndef WIN32
      struct timeval t;
      struct timezone z;
      gettimeofday(&t, &z);
      long unique = t.tv_usec;
#else
      QTime qt = QTime::currentTime();
      long unique = qt.msec() + (qt.second() + qt.minute() * 60) * 1000;
#endif

      int indent = 0;

      QFileInfo fileName(fileName_);
      QString includeGuard(fileName.baseName() + "_" + extensionName_);

      ostr << head << std::endl;
      ostr << "#ifndef " << includeGuard
      << "_" << unique // make it truely unique
      << std::endl
      << "#define " << includeGuard
      << "_" << unique // make it truely unique
      << std::endl;

      QStringSet::const_iterator i;
      if (include_.size() > 0)
        ostr << std::endl;
      for (i = include_.begin(); i != include_.end(); ++i)
        ostr << "#include \"" << *i << "\"" << std::endl;
      if (Include_.size() > 0)
        ostr << std::endl;
      for (i = Include_.begin(); i != Include_.end(); ++i)
        ostr << "#include <" << *i << ">" << std::endl;

      if (!exportDirective.isEmpty()) {
        ostr << "\n// ACE dll export directive header" << std::endl;
        ostr << "#include \"" << QSTRCAST(exportDirective) << ".h\"" << std::endl;
      }

      ostr << std::endl;

      if (forwardDeclaration_.size() != 0) {
        ostr << "// forward declarations" << std::endl;
        for (i = forwardDeclaration_.begin(); i != forwardDeclaration_.end(); ++i)
          ostr << "class " << *i << ";" << std::endl;
        ostr << std::endl;
      }

      QStringVector::const_iterator f, l = namespace_.end();
      for (f = namespace_.begin(); f != l; ++f) {
        ostr << spaces.left(indent) << "namespace " <<  *f << std::endl
        << spaces.left(indent) << "{" << std::endl;
        indent += STEP;
      }

      if (localForwardDeclaration_.size() != 0) {
        ostr << "// forward declarations" << std::endl;
        for (i = localForwardDeclaration_.begin(); i != localForwardDeclaration_.end(); ++i)
          ostr << "class " << *i << ";" << std::endl;
        ostr << std::endl;
      }

      TypeVector::const_iterator first, last = type_.end();
      for (first = type_.begin(); first != last; ++first) {
        if (first != type_.begin())
          ostr << std::endl;
        if (!first->isDummy())
          first->generateHeader(ostr, indent, exportDirective);
      }

      // close namespace scope
      while (indent) {
        indent -= STEP;
        ostr << spaces.left(indent) << "}" << std::endl;
      }

      ostr << std::endl
      << "#endif // " << fileName_ << "_" << extensionName_ << std::endl;
    }

    void
    Generator::generateSource(std::ostream& ostr) const
    {
      if (fileName_.isEmpty())
        throw QString("No file name specified.");

      int indent = 0;

      ostr << head << std::endl;
      ostr << "#include \"" << fileName_ << "." << extensionName_ << "\"" << std::endl
      << std::endl;

      QStringSet::const_iterator i;
      for (i = srcInclude_.begin(); i != srcInclude_.end(); ++i)
        ostr << "#include <" << *i << ">" << std::endl;

      ostr << "#include <miro/XmlParse.h>" << std::endl
      << "#include <qdom.h>" << std::endl
      << std::endl;

      ostr << "#ifdef  HAVE_CONFIG_H\n"
      "#include \"config.h\"\n"
      "#endif//HAVE_CONFIG_H\n\n";

      // match the operator <<= from Miro into local namespace
      if (namespace_.size() == 0 ||
            namespace_.front() != QString("Miro")) {
        ostr << "namespace" << std::endl
        << "{" << std::endl
        << "  using Miro::operator<<=;" << std::endl
        << "  using Miro::operator>>=;" << std::endl
        << "  using Miro::operator<<;" << std::endl
        << "};" << std::endl
        << std::endl;
      }

      QStringVector::const_iterator f, l = namespace_.end();
      for (f = namespace_.begin(); f != l; ++f) {
        ostr << spaces.left(indent) << "namespace " <<  *f << std::endl
        << spaces.left(indent) << "{" << std::endl;
        indent += STEP;
      }

      TypeVector::const_iterator first, last = type_.end();
      for (first = type_.begin(); first != last; ++first) {
        if (first != type_.begin())
          ostr << std::endl;
        if (!first->isDummy())
          first->generateSource(ostr, indent);
      }

      while (indent) {
        indent -= STEP;
        ostr << spaces.left(indent) << "}" << std::endl;
      }

    }

    QStringVector
    Generator::groups() const
    {
      QStringVector result;
      result.reserve(groups_.size());
      GroupMap::const_iterator first, last = groups_.end();
      for (first = groups_.begin(); first != last; ++first) {
        result.push_back(first->first);
#if QT_VERSION >= 0x040000
        result.back()[0] = result.back()[0].toUpper();
#else // QT_VERSION >= 0x040000
        result.back()[0] = result.back()[0].upper();
#endif // QT_VERSION >= 0x040000
      }
      std::sort(result.begin(), result.end());
      // FIXME: this should actually
      QStringVector::iterator i = std::unique(result.begin(), result.end());
      result.erase(i, result.end());
      return result;
    }

    void
    Generator::getGroupedTypes(const QString& _group,
                               GroupMap::const_iterator& _first, GroupMap::const_iterator& _last) const
    {
      _first = groups_.lower_bound(_group);
      _last = groups_.upper_bound(_group);
    }

    const Type *
    Generator::getType(const QString& _name) const
    {
      TypeVector::const_iterator first, last = type_.end();
      for (first = type_.begin(); first != last; ++first)
        if (first->name() == _name ||
              first->name() + "Parameters" == _name ||
              first->fullName() == _name ||
              first->fullName() == _name + "Parameters") {

#ifdef GCC_MAJOR_VERSION
#if GCC_MAJOR_VERSION > 2
          return first.base();
#else
          return first;
#endif // GCC_MAJOR_VERSION > 2
#else
          return &*first;
#endif // GCC_MAJOR_VERSION

        }
      return NULL;
    }

    void
    Generator::clearNamespace()
    {
      namespace_.clear();
    }

    ParameterVector
    Generator::getFullParameterSet(Type const& _type) const
    {
      ParameterVector params;

      // add all superclass members to the parameter set
      // start with the root parameter set
      if (_type.parent().isEmpty()) {
        params = _type.parameterSet();
      }
      else {
        Miro::CFG::Type const * parent = getType(_type.parent());
        if (parent == NULL) {
          throw QString("Parameter description for " +
                        _type.parent() +
                        " not found.\nCheck whether the relevant description file is loaded.");
        }
        params = getFullParameterSet(*parent);

        ParameterVector::const_iterator first, last = _type.parameterSet().end();
        for (first = _type.parameterSet().begin(); first != last; ++first) {
          ParameterVector::iterator i =
            std::find(params.begin(), params.end(), *first);

          // add parameter to set
          if (i == params.end()) {
            params.push_back(*first);
          } // overwrite default value of inherited parameter
          else {
            i->default_ = first->default_;
          }
        }
      }

      return params;
    }

    ParameterVector
    Generator::getFullStaticConstParameterSet(Type const& _type) const
    {
      ParameterVector params;

      // add all superclass members to the parameter set
      // start with the root parameter set
      if (_type.parent().isEmpty()) {
        params = _type.staticConstParameterSet();
      }
      else {
        Miro::CFG::Type const * parent = getType(_type.parent());
        if (parent == NULL) {
          throw QString("Parameter description for " +
                        _type.parent() +
                        " not found.\nCheck whether the relevant description file is loaded.");
        }

        params = getFullStaticConstParameterSet(*parent);
        params.insert(params.end(),
                      _type.staticConstParameterSet().begin(),
                      _type.staticConstParameterSet().end());
      }

      return params;
    }

    bool
    Generator::isDerivedType(Type const& _type, Type const& _ancestor) const
    {
      if (_type.fullName() == _ancestor.fullName())
        return true;

      if (!_type.parent().isEmpty()) {
        Miro::CFG::Type const * parent = getType(_type.parent());
        if (parent != NULL) {
          return isDerivedType(*parent, _ancestor);
        }
      }

      return false;
    }

    QStringVector
    Generator::getDescendants(Type const& _type) const
    {
      QStringVector descendants;
      descendants.reserve(type_.size() / 2);

      Generator::TypeVector::const_iterator first, last = type_.end();
      for (first = type_.begin(); first != last; ++first) {
        if (isDerivedType(*first, _type)) {
          descendants.push_back(first->fullName());
        }
      }

      std::sort(descendants.begin(), descendants.end());
      return descendants;
    }

    std::ostream& operator << (std::ostream& _ostr, const Generator& _rhs)
    {
      Generator::TypeVector::const_iterator first, last = _rhs.type_.end();
      for (first = _rhs.type_.begin(); first != last; ++first) {
        _ostr << first->fullName() << std::endl;
        for (unsigned int i = 0; i < first->staticConstParameterSet().size(); ++i) {
          _ostr << " static const param " << first->staticConstParameterSet()[i].name_ << std::endl;
        }
      }
      return _ostr;
    }
  }
}
