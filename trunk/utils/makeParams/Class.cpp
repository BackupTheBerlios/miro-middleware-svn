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

#include "Class.h"

#include <iostream>

namespace 
{
  QString spaces("                                                        ");
}

void
Class::addStatic(const QString& _type, const QString& _name)
{
  staticData_.push_back(make_pair(_type, _name));
}

void
Class::setName(const QString& _name)
{
  name_ = _name;
}

void
Class::setParent(const QString& _parent)
{
  parent_ = _parent;
}

void 
Class::addToConstructor(const QString& _ctor)
{
  ctor_.push_back(_ctor);
}

void 
Class::addParameter(const Parameter& _parameter)
{
  parameter_.insert(_parameter);
}

void
Class::generateHeader(ostream& ostr, unsigned long _indent) const
{
  if(name_.isEmpty())
    throw QString("No class name specified.");

  unsigned long indent = _indent;
  QString spaces("                                                        ");

  // debug ostream operator
  if (parent_.isEmpty()) {
    ostr << spaces.left(indent) << "struct " << name_ << ";" << endl
	 << spaces.left(indent) << "std::ostream&" << endl
	 << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "& rhs);" << endl
	 << endl;
  }

  ostr << spaces.left(indent) << "struct " << name_;
  if (!parent_.isEmpty())
    ostr << " : " << parent_;

  ostr << endl
       << spaces.left(indent) << "{" << endl;
  indent += STEP;
  
  if (!parent_.isEmpty())
    ostr << spaces.left(indent) << "typedef " << parent_ << " Super;" << endl
	 << endl;

  // data members
  ParameterSet::const_iterator j;
  for (j = parameter_.begin(); j != parameter_.end(); ++j)
    ostr << spaces.left(indent) << j->type_ << " " << j->name_ << ";" << endl;
  if (parameter_.size() != 0)
    ostr << endl;

  // static data members
  QStringPairVector::const_iterator k;
  for (k = staticData_.begin(); k != staticData_.end(); ++k)
    ostr << spaces.left(indent) << "static " << k->first << " " << k->second << ";" << endl;
  if (staticData_.size() != 0)
    ostr << endl;
 
  // constructor
  ostr << spaces.left(indent) << name_ << "();" << endl;
  // destructor
  if (parent_.isEmpty())
    ostr << spaces.left(indent) << "virtual ~"<< name_ << "();" << endl;
  // parsing operator
  ostr << endl
       << spaces.left(indent) << "virtual void operator <<= (const QDomNode& _node);" << endl;
  // debug output operator
  ostr << spaces.left(indent) << "virtual void printToStream(std::ostream& _ostr) const;" << endl;

  while (indent > _indent) {
    indent -= STEP;
    ostr << spaces.left(indent) << "};" << endl;
  }
}

void
Class::generateSource(ostream& ostr, unsigned long _indent) const
{
  if(name_.isEmpty())
    throw QString("No class name specified.");

  unsigned long indent = _indent;

  QStringVector::const_iterator i;
  ParameterSet::const_iterator j;

  // debug ostream operator
  if (parent_.isEmpty()) {
    ostr << spaces.left(indent) << "std::ostream&" << endl
	 << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "& rhs)" << endl
	 << spaces.left(indent) << "{" << endl;
    indent += STEP;
    ostr << spaces.left(indent) << "rhs.printToStream(ostr);" << endl
	 << spaces.left(indent) << "return ostr;" << endl;
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl
	 << endl;
  }

  // static data members
  QStringPairVector::const_iterator k;
  for (k = staticData_.begin(); k != staticData_.end(); ++k)
    ostr << spaces.left(indent) << k->first << " " << name_ << "::" << k->second << ";" << endl;
  if (staticData_.size() != 0)
    ostr << endl;

  // constructor
  ostr << spaces.left(indent) << name_ << "::" << name_ << "()";
  if (parameter_.size() > 0) {
    ostr << " :";
    indent += 2;
 
    for (j = parameter_.begin(); j != parameter_.end(); ++j) {
      if (j != parameter_.begin())
	ostr << ",";
      ostr << endl
	   << spaces.left(indent) << j->name_ << "(";
      if (!j->default_.isEmpty())
	ostr << j->default_;
      ostr << ")";
    }
    indent -= 2;
  }
  ostr << endl 
       << spaces.left(indent) << "{" << endl;

  indent += STEP;
  for (i = ctor_.begin(); i != ctor_.end(); ++i)
    ostr << spaces.left(indent) << *i << endl;
  indent -= STEP;

  ostr << spaces.left(indent) << "}" << endl
       << endl;

  // destructor
  if (parent_.isEmpty()) 
    ostr << spaces.left(indent) << name_ << "::~" << name_ << "()" << endl
	 << spaces.left(indent) << "{}" << endl
	 << endl;

  // operator ()
  unsigned long preIndent = indent;
  ostr << spaces.left(indent) << "void" << endl
       << spaces.left(indent) << name_ << "::operator <<= (const QDomNode& _node)" << endl
       << spaces.left(indent) << "{" << endl;
  indent += STEP;

  if (!parent_.isEmpty())
    ostr << spaces.left(indent) << "Super::operator <<= (_node);" << endl
	 << endl;
  if (parameter_.size() > 0) {
    ostr << spaces.left(indent) << "if (!_node.isNull()) {" << endl;
    indent += STEP;
    ostr << spaces.left(indent) << "QDomNode n = _node.firstChild();" << endl
	 << spaces.left(indent) << "while(!n.isNull() ) {" << endl;
    indent += STEP;

    for (j = parameter_.begin(); j != parameter_.end(); ++j) {
      QString name(j->name_);
      name[0] = name[0].upper();
      ostr << spaces.left(indent);
      if (j != parameter_.begin())
	ostr << "else ";
      ostr << "if (n.nodeName() == \"" << name << "\")" << endl;
      indent += STEP;
      ostr << spaces.left(indent) << j->name_ << " <<= n;" << endl;
      indent -= STEP;
    }

    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl
	 << spaces.left(indent) << "n = n.nextSibling();" << endl;
  }
  while(indent > preIndent) {
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl;
  }

  // stream output

  ostr << endl
       << spaces.left(indent) << "void" << endl
       << spaces.left(indent) << name_ <<"::printToStream(std::ostream& ostr) const" << endl
       << spaces.left(indent) << "{" << endl;
  indent += STEP;

  if (!parent_.isEmpty())
    ostr << spaces.left(indent) << "Super::printToStream(ostr);" << endl
	 << endl;

  if (parameter_.size() > 0) {
    ostr << spaces.left(indent) << "ostr << ";
    indent += 5;
    for (j = parameter_.begin(); j != parameter_.end(); ++j) {
      if (j != parameter_.begin())
	ostr << endl
	     << spaces.left(indent) << "<< ";
      if (!j->description_.isEmpty())
	ostr << "\"" << j->description_;
      else {
	QString name(j->name_);
	name[0] = name[0].upper();

	ostr << "\"" << name;
      }
      ostr << ": \" << " << j->name_;
      if (!j->measure_.isEmpty())
	ostr << " << \"" << j->measure_ << "\"";
      ostr << " << endl";
    }
    ostr << ";" << endl;
    indent -= 5;
  }

  indent -= STEP;
  ostr << spaces.left(indent) << "}" << endl;
}

const QString&
Class::name() const
{
  return name_;
}
