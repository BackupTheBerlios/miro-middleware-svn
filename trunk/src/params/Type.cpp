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

#include "Type.h"

#include <iostream>
#include <algorithm>

namespace 
{
  QString spaces("                                                        ");
}

namespace Miro
{
  namespace CFG
  {
    Type::Type() :
      final_(true),
      dummy_(false),
      extern_(false),
      instance_(false)
    {}

    void
    Type::addStatic(const QString& _type, const QString& _name)
    {
      staticData_.push_back(std::make_pair(_type, _name));
    }

    void 
    Type::addToConstructor(const QString& _ctor)
    {
      ctor_.push_back(_ctor);
    }

    void 
    Type::addParameter(const Parameter& _parameter)
    {
      ParameterVector::const_iterator i =
	std::find(parameter_.begin(), parameter_.end(), _parameter);
      if (i == parameter_.end())
	parameter_.push_back(_parameter);
      else {
	throw QString("Parameter multiply defined for type " + 
		      fullName() + 
		      ": " + _parameter.name_);

      }
    }

    void
    Type::generateHeader(std::ostream& ostr, unsigned long _indent) const
    {
      //   if(name_.isEmpty())
      //     throw QString("No class name specified.");

      unsigned long indent = _indent;

      if (!isExtern()) {
	// debug ostream operator
	if (parent_.isEmpty()) {
	  ostr << spaces.left(indent) << "struct " << name_ << "Parameters;" << std::endl
	       << spaces.left(indent) << "std::ostream&" << std::endl
	       << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs);" << std::endl
	       << std::endl;
	}

	ostr << spaces.left(indent) << "class " << name_ << "Parameters";
	if (!parent_.isEmpty())
	  ostr << " : public " << parent_;

	ostr << std::endl
	     << spaces.left(indent) << "{" << std::endl;
	indent += STEP;
  
	if (!parent_.isEmpty())
	  ostr << spaces.left(indent) << "typedef " << parent_ << " Super;" << std::endl
	       << std::endl;

	ostr << spaces.left(indent - STEP) << "public: " << std::endl;

	// data members
	ParameterVector::const_iterator j;
	for (j = parameter_.begin(); j != parameter_.end(); ++j)
	  ostr << spaces.left(indent) 
	       << ((j->type_ != "angle")? j->type_ : QString("double")) 
	       << " " << j->name_ << ";" << std::endl;
	if (parameter_.size() != 0)
	  ostr << std::endl;

	// static data members
	QStringPairVector::const_iterator k;
	for (k = staticData_.begin(); k != staticData_.end(); ++k)
	  ostr << spaces.left(indent) << "static " << k->first << " " << k->second << ";" << std::endl;
	if (staticData_.size() != 0)
	  ostr << std::endl;
 
	// constructor
	ostr << spaces.left(indent) << name_ << "Parameters();" << std::endl;
	// destructor
	if (parent_.isEmpty())
	  ostr << spaces.left(indent) << "virtual ~"<< name_ << "Parameters();" << std::endl;
	// parsing operator
	ostr << std::endl;
	if (parameter_.size() > 0 || parent_.isEmpty()) {
	  ostr << spaces.left(indent) << "virtual void operator <<= (const QDomNode&);" << std::endl;
	  ostr << spaces.left(indent) << "virtual QDomElement operator >>= (QDomNode&) const;" << std::endl;
	}
	// debug output operator
	if (parameter_.size() > 0 || parent_.isEmpty()) {
	  ostr << spaces.left(indent - STEP) << "protected:" << std::endl
	       << spaces.left(indent) << "virtual void printToStream(std::ostream&) const;" << std::endl;
	}

	if (parent_.isEmpty())
	  ostr << spaces.left(indent) << "friend" << std::endl
	       << spaces.left(indent) << "std::ostream&" << std::endl
	       << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs);" << std::endl;
      }
      else {
	// if this is a externally defined class, 
	// use global operators for parsing
	ostr << spaces.left(indent) << "void operator <<= (" << name_ << "&, const QDomNode&);" << std::endl;
	ostr << spaces.left(indent) << "QDomElement operator >>= (const " << name_ << "&, QDomNode&);" << std::endl;

      }

      while (indent > _indent) {
	indent -= STEP;
	ostr << spaces.left(indent) << "};" << std::endl;
      }
    }

    void
    Type::generateSource(std::ostream& ostr, unsigned long _indent) const
    {
      //   if(name_.isEmpty())
      //     throw QString("No class name specified.");

      unsigned long indent = _indent;

      if (!isExtern()) {
	QStringVector::const_iterator i;
	ParameterVector::const_iterator j;
  
	// debug ostream operator
	if (parent_.isEmpty()) {
	  ostr << spaces.left(indent) << "std::ostream&" << std::endl
	       << spaces.left(indent) << "operator << (std::ostream& ostr, const " << name_ << "Parameters& rhs)" << std::endl
	       << spaces.left(indent) << "{" << std::endl;
	  indent += STEP;
	  ostr << spaces.left(indent) << "rhs.printToStream(ostr);" << std::endl
	       << spaces.left(indent) << "return ostr;" << std::endl;
	  indent -= STEP;
	  ostr << spaces.left(indent) << "}" << std::endl
	       << std::endl;
	}
  
	// static data members
	QStringPairVector::const_iterator k;
	for (k = staticData_.begin(); k != staticData_.end(); ++k)
	  ostr << spaces.left(indent) << k->first << " " << name_ << "Parameters::" << k->second << ";" << std::endl;
	if (staticData_.size() != 0)
	  ostr << std::endl;
  
	// constructor
	ostr << spaces.left(indent) << name_ << "Parameters::" << name_ << "Parameters()";
	if (parameter_.size() > 0) {
	  ostr << " :";
	  indent += 2;
    
	  for (j = parameter_.begin(); j != parameter_.end(); ++j) {
	    if (j != parameter_.begin())
	      ostr << ",";
	    ostr << std::endl
		 << spaces.left(indent) << j->name_ << "(" << j->fullDefault_ <<")";
	  }
	  indent -= 2;
	}
	ostr << std::endl 
	     << spaces.left(indent) << "{" << std::endl;
  
	indent += STEP;
	for (i = ctor_.begin(); i != ctor_.end(); ++i)
	  ostr << spaces.left(indent) << *i << std::endl;
	indent -= STEP;
  
	ostr << spaces.left(indent) << "}" << std::endl
	     << std::endl;
  
	// destructor
	if (parent_.isEmpty()) 
	  ostr << spaces.left(indent) << name_ << "Parameters::~" << name_ << "Parameters()" << std::endl
	       << spaces.left(indent) << "{}" << std::endl
	       << std::endl;
  
	if (parameter_.size() > 0 || parent_.isEmpty()) {

	  // operator <<=
	  ostr << spaces.left(indent) << "void" << std::endl
	       << spaces.left(indent) << name_ << "Parameters::operator <<= (const QDomNode&";
	  if (parameter_.size() > 0 || !parent_.isEmpty()) 
	    ostr <<" _node";
	  ostr << ")" << std::endl;
    
	  generateQDomOutOperator(ostr, "this->", indent);
    
	  // operator >>=
	  ostr << spaces.left(indent) << "QDomElement" << std::endl
	       << spaces.left(indent) << name_ << "Parameters::operator >>= (QDomNode& _node) const" << std::endl;
	  generateQDomInOperator(ostr, "this->", indent);
    
	  // stream output
	  ostr << std::endl
	       << spaces.left(indent) << "void" << std::endl
	       << spaces.left(indent) << name_ <<"Parameters::printToStream(std::ostream&";
	  if (parameter_.size() > 0 || !parent_.isEmpty())
	    ostr << " ostr";
	  ostr << ") const" << std::endl
	       << spaces.left(indent) << "{" << std::endl;
	  indent += STEP;

	  if (!parent_.isEmpty())
	    ostr << spaces.left(indent) << "Super::printToStream(ostr);" << std::endl
		 << std::endl;

	  if (parameter_.size() > 0) {
	    ostr << spaces.left(indent) << "ostr << ";
	    indent += 5;
	    for (j = parameter_.begin(); j != parameter_.end(); ++j) {
	      if (j != parameter_.begin())
		ostr << std::endl
		     << spaces.left(indent) << "<< ";
	      if (!j->description_.isEmpty())
		ostr << "\"" << j->description_;
	      else {
		QString name(j->name_);
		name[0] = name[0].upper();
	  
		ostr << "\"" << name;
	      }
	      ostr << ": \" << ";

	      if (j->type_ != "angle")
		ostr << j->name_;
	      else
		ostr << "Miro::rad2Deg(" << j->name_ << ")" << std::endl;

	      if (!j->measure_.isEmpty())
		ostr << " << \"" << j->measure_ << "\"";
	      ostr << " << std::endl";
	    }
	    ostr << ";" << std::endl;
	    indent -= 5;
	  }
	  indent -= STEP;
	  ostr << spaces.left(indent) << "}" << std::endl;
	}
      }
      else {
	// operator <<=
	ostr << spaces.left(indent) << "void" << std::endl
	     << spaces.left(indent) << "operator<<= (" << name_ <<"& _lhs, const QDomNode& _node)" << std::endl;
    
	generateQDomOutOperator(ostr, "_lhs.", indent);
    
	// operator >>=
	ostr << spaces.left(indent) << "QDomElement" << std::endl
	     << spaces.left(indent) << "operator>>= (const " << name_ <<"& _lhs, QDomNode& _node)" << std::endl;
	generateQDomInOperator(ostr, "_lhs.", indent);
      }
    }

    // operator <<=
    void
    Type::generateQDomOutOperator(std::ostream& ostr, 
				  const QString& classPrefix, unsigned long indent) const
    {
      unsigned long preIndent = indent;

      ostr << spaces.left(indent) << "{" << std::endl;
      indent += STEP;
    
      if (!parent_.isEmpty())
	ostr << spaces.left(indent) << classPrefix << "Super::operator <<= (_node);" << std::endl
	     << std::endl;
      if (parameter_.size() > 0) {
	ostr << spaces.left(indent) << "if (!_node.isNull()) {" << std::endl;
	indent += STEP;
	ostr << spaces.left(indent) << "QDomNode n = _node.firstChild();" << std::endl
	     << spaces.left(indent) << "while(!n.isNull() ) {" << std::endl;
	indent += STEP;
      
	ostr << spaces.left(indent) << "QDomElement e = n.toElement();" << std::endl
	     << spaces.left(indent) << "if( !e.isNull() ) {" << std::endl;
	indent += STEP;
      
	ostr << spaces.left(indent) << "QDomAttr a = e.attributeNode(\"name\");" << std::endl
	     << spaces.left(indent) << "if (!a.isNull()) {" << std::endl;
	indent += STEP;
      
	ostr << spaces.left(indent) << "QString i = a.value();" << std::endl;
      
	ParameterVector::const_iterator j;
	for (j = parameter_.begin(); j != parameter_.end(); ++j) {
	  QString name(j->name_);
	  name[0] = name[0].upper();
	  ostr << spaces.left(indent);
	  if (j != parameter_.begin())
	    ostr << "else ";
	  ostr << "if (i == \"" << name << "\") {" << std::endl;
	  indent += STEP;
	  ostr << spaces.left(indent) << classPrefix << j->name_ << " <<= n;" << std::endl;
	  if (j->type_ == "angle")
	    ostr << spaces.left(indent) << classPrefix << j->name_ 
		 << " = Miro::deg2Rad(" << classPrefix << j->name_ << ");" << std::endl;

	  indent -= STEP;
	  ostr << spaces.left(indent) << "}" << std::endl;
	}
      
	indent -= STEP;
	ostr << spaces.left(indent) << "}" << std::endl;
	indent -= STEP;
	ostr << spaces.left(indent) << "}" << std::endl
	     << spaces.left(indent) << "n = n.nextSibling();" << std::endl;
	indent -= STEP;
	ostr << spaces.left(indent) << "}" << std::endl;
      }
      while(indent > preIndent) {
	indent -= STEP;
	ostr << spaces.left(indent) << "}" << std::endl;
      }
    }

    // operator >>=
    void
    Type::generateQDomInOperator(std::ostream& ostr, 
				 const QString& classPrefix, unsigned long indent) const
    {
      ostr << spaces.left(indent) << "{" << std::endl;
      indent += STEP;
    
      if (parent_.isEmpty()) {
	ostr << spaces.left(indent) << "QDomDocument d = _node.ownerDocument();" << std::endl;
	ostr << spaces.left(indent) << "QDomElement e = d.createElement(\"parameter\");" << std::endl;
	ostr << spaces.left(indent) << "_node.appendChild(e);" << std::endl << std::endl;
      }
      else {
	ostr << spaces.left(indent) << "QDomElement e = Super::operator >>= (_node);" << std::endl;
      }

      ostr << spaces.left(indent) << "QDomElement g;" << std::endl << std::endl;

      ParameterVector::const_iterator j;
      for (j = parameter_.begin(); j != parameter_.end(); ++j) {
	QString name(j->name_);
	name[0] = name[0].upper();

	if (j->type_ != "angle")
	  ostr << spaces.left(indent) << "g = (" << classPrefix << j->name_ << " >>= e);" << std::endl;
	else
	  ostr << spaces.left(indent) << "g = (Miro::rad2Deg(" << classPrefix << j->name_ << ") >>= e);" << std::endl;
      
	ostr << spaces.left(indent) << "g.setAttribute(\"name\", \"" << name <<"\");" << std::endl;
      }
      
      ostr << std::endl
	   << spaces.left(indent) << "return e;" << std::endl;
      indent -= STEP;
      ostr << spaces.left(indent) << "}" << std::endl;
    }
  }
}
