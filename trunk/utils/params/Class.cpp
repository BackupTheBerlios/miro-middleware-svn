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

Class::Class() :
  final_(true),
  dummy_(false),
  extern_(false)
{}

void
Class::addStatic(const QString& _type, const QString& _name)
{
  staticData_.push_back(make_pair(_type, _name));
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
//   if(name_.isEmpty())
//     throw QString("No class name specified.");

  unsigned long indent = _indent;

  if (!isExtern()) {
  // debug ostream operator
  if (parent_.isEmpty()) {
    ostr << spaces.left(indent) << "struct " << name_ << "Parameters;" << endl
	 << spaces.left(indent) << "std::ostream&" << endl
	 << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs);" << endl
	 << endl;
  }

  ostr << spaces.left(indent) << "class " << name_ << "Parameters";
  if (!parent_.isEmpty())
    ostr << " : public " << parent_;

  ostr << endl
       << spaces.left(indent) << "{" << endl;
  indent += STEP;
  
  if (!parent_.isEmpty())
    ostr << spaces.left(indent) << "typedef " << parent_ << " Super;" << endl
	 << endl;

  ostr << spaces.left(indent - STEP) << "public: " << endl;

  // data members
  ParameterSet::const_iterator j;
  for (j = parameter_.begin(); j != parameter_.end(); ++j)
    ostr << spaces.left(indent) 
	 << ((j->type_ != "angle")? j->type_ : QString("double")) 
	 << " " << j->name_ << ";" << endl;
  if (parameter_.size() != 0)
    ostr << endl;

  // static data members
  QStringPairVector::const_iterator k;
  for (k = staticData_.begin(); k != staticData_.end(); ++k)
    ostr << spaces.left(indent) << "static " << k->first << " " << k->second << ";" << endl;
  if (staticData_.size() != 0)
    ostr << endl;
 
  // constructor
  ostr << spaces.left(indent) << name_ << "Parameters();" << endl;
  // destructor
  if (parent_.isEmpty())
    ostr << spaces.left(indent) << "virtual ~"<< name_ << "Parameters();" << endl;
  // parsing operator
  ostr << endl;
  if (parameter_.size() > 0 || parent_.isEmpty()) {
    ostr << spaces.left(indent) << "virtual void operator <<= (const QDomNode&);" << endl;
    ostr << spaces.left(indent) << "virtual QDomElement operator >>= (QDomNode&) const;" << endl;
  }
  // debug output operator
  if (parameter_.size() > 0 || parent_.isEmpty()) {
    ostr << spaces.left(indent - STEP) << "protected:" << endl
	 << spaces.left(indent) << "virtual void printToStream(std::ostream&) const;" << endl;
  }

  if (parent_.isEmpty())
    ostr << spaces.left(indent) << "friend" << endl
	 << spaces.left(indent) << "std::ostream&" << endl
	 << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs);" << endl;
  }
  else {
    // if this is a externally defined class, 
    // use global operators for parsing
    ostr << spaces.left(indent) << "void operator <<= (" << name_ << "&, const QDomNode&);" << endl;
    ostr << spaces.left(indent) << "QDomElement operator >>= (const " << name_ << "&, QDomNode&);" << endl;

  }

  while (indent > _indent) {
    indent -= STEP;
    ostr << spaces.left(indent) << "};" << endl;
  }
}

void
Class::generateSource(ostream& ostr, unsigned long _indent) const
{
//   if(name_.isEmpty())
//     throw QString("No class name specified.");

  unsigned long indent = _indent;

  if (!isExtern()) {
    QStringVector::const_iterator i;
    ParameterSet::const_iterator j;
  
    // debug ostream operator
    if (parent_.isEmpty()) {
      ostr << spaces.left(indent) << "std::ostream&" << endl
	   << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs)" << endl
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
      ostr << spaces.left(indent) << k->first << " " << name_ << "Parameters::" << k->second << ";" << endl;
    if (staticData_.size() != 0)
      ostr << endl;
  
    // constructor
    ostr << spaces.left(indent) << name_ << "Parameters::" << name_ << "Parameters()";
    if (parameter_.size() > 0) {
      ostr << " :";
      indent += 2;
    
      for (j = parameter_.begin(); j != parameter_.end(); ++j) {
	if (j != parameter_.begin())
	  ostr << ",";
	ostr << endl
	     << spaces.left(indent) << j->name_ << "(" << j->fullDefault_ <<")";
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
      ostr << spaces.left(indent) << name_ << "Parameters::~" << name_ << "Parameters()" << endl
	   << spaces.left(indent) << "{}" << endl
	   << endl;
  
    if (parameter_.size() > 0 || parent_.isEmpty()) {

      // operator <<=
      ostr << spaces.left(indent) << "void" << endl
	   << spaces.left(indent) << name_ << "Parameters::operator <<= (const QDomNode&";
      if (parameter_.size() > 0 || !parent_.isEmpty()) 
	ostr <<" _node";
      ostr << ")" << endl;
    
      generateQDomOutOperator(ostr, "this->", indent);
    
      // operator >>=
      ostr << spaces.left(indent) << "QDomElement" << endl
	   << spaces.left(indent) << name_ << "Parameters::operator >>= (QDomNode& _node) const" << endl;
      generateQDomInOperator(ostr, "this->", indent);
    
      // stream output
      ostr << endl
	   << spaces.left(indent) << "void" << endl
	   << spaces.left(indent) << name_ <<"Parameters::printToStream(std::ostream&";
      if (parameter_.size() > 0 || !parent_.isEmpty())
	ostr << " ostr";
      ostr << ") const" << endl
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
  }
  else {
    // operator <<=
    ostr << spaces.left(indent) << "void" << endl
	 << spaces.left(indent) << "operator<<= (" << name_ <<"& _lhs, const QDomNode& _node)" << endl;
    
    generateQDomOutOperator(ostr, "_lhs.", indent);
    
    // operator >>=
    ostr << spaces.left(indent) << "QDomElement" << endl
	 << spaces.left(indent) << "operator>>= (const " << name_ <<"& _lhs, QDomNode& _node)" << endl;
    generateQDomInOperator(ostr, "_lhs.", indent);
  }
}

// operator <<=
void
Class::generateQDomOutOperator(std::ostream& ostr, 
			       const QString& classPrefix, unsigned long indent) const
{
  unsigned long preIndent = indent;

  ostr << spaces.left(indent) << "{" << endl;
  indent += STEP;
    
  if (!parent_.isEmpty())
    ostr << spaces.left(indent) << classPrefix << "Super::operator <<= (_node);" << endl
	 << endl;
  if (parameter_.size() > 0) {
    ostr << spaces.left(indent) << "if (!_node.isNull()) {" << endl;
    indent += STEP;
    ostr << spaces.left(indent) << "QDomNode n = _node.firstChild();" << endl
	 << spaces.left(indent) << "while(!n.isNull() ) {" << endl;
    indent += STEP;
      
    ostr << spaces.left(indent) << "QDomElement e = n.toElement();" << endl
	 << spaces.left(indent) << "if( !e.isNull() ) {" << endl;
    indent += STEP;
      
    ostr << spaces.left(indent) << "QDomAttr a = e.attributeNode(\"name\");" << endl
	 << spaces.left(indent) << "if (!a.isNull()) {" << endl;
    indent += STEP;
      
    ostr << spaces.left(indent) << "QString i = a.value();" << endl;
      
    ParameterSet::const_iterator j;
    for (j = parameter_.begin(); j != parameter_.end(); ++j) {
      QString name(j->name_);
      name[0] = name[0].upper();
      ostr << spaces.left(indent);
      if (j != parameter_.begin())
	ostr << "else ";
      ostr << "if (i == \"" << name << "\") {" << endl;
      indent += STEP;
      ostr << spaces.left(indent) << classPrefix << j->name_ << " <<= n;" << endl;
      if (j->type_ == "angle")
	ostr << spaces.left(indent) << classPrefix << j->name_ 
	     << " = Miro::deg2Rad(" << classPrefix << j->name_ << ");" << endl;

      indent -= STEP;
      ostr << spaces.left(indent) << "}" << endl;
    }
      
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl;
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl
	 << spaces.left(indent) << "n = n.nextSibling();" << endl;
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl;
  }
  while(indent > preIndent) {
    indent -= STEP;
    ostr << spaces.left(indent) << "}" << endl;
  }
}

// operator >>=
void
Class::generateQDomInOperator(std::ostream& ostr, 
			      const QString& classPrefix, unsigned long indent) const
{
  ostr << spaces.left(indent) << "{" << endl;
  indent += STEP;
    
  if (parent_.isEmpty()) {
    ostr << spaces.left(indent) << "QDomDocument d = _node.ownerDocument();" << endl;
    ostr << spaces.left(indent) << "QDomElement e = d.createElement(\"parameter\");" << endl;
    ostr << spaces.left(indent) << "_node.appendChild(e);" << endl << endl;
  }
  else {
    ostr << spaces.left(indent) << "QDomElement e = Super::operator >>= (_node);" << endl;
  }

  ostr << spaces.left(indent) << "QDomElement g;" << endl << endl;

  ParameterSet::const_iterator j;
  for (j = parameter_.begin(); j != parameter_.end(); ++j) {
    QString name(j->name_);
    name[0] = name[0].upper();

    if (j->type_ != "angle")
      ostr << spaces.left(indent) << "g = (" << classPrefix << j->name_ << " >>= e);" << endl;
    else
      ostr << spaces.left(indent) << "g = (Miro::rad2Deg(" << classPrefix << j->name_ << ") >>= e);" << endl;
      
    ostr << spaces.left(indent) << "g.setAttribute(\"name\", \"" << name <<"\");" << endl;
  }
      
  ostr << endl
       << spaces.left(indent) << "return e;" << endl;
  indent -= STEP;
  ostr << spaces.left(indent) << "}" << endl;
}
