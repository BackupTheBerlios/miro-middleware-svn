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

#include "Parser.h"
#include "Generator.h"
#include "Parameter.h"

#include "miro/Exception.h"

#include <qstring.h>

Parser::Parser(Generator& _generator) :
  class_(),
  generator_(_generator),
  parsing_(false),
  string_(false),
  timeValue_(false),
  serialParams_(false),
  scanDescription_(false)
{
}

bool Parser::startDocument()
{
  return true;
}

bool Parser::startElement( const QString&, 
			   const QString&, 
			   const QString& qName, 
			   const QXmlAttributes& attributes)
{
  QString str;

  if (qName == "config_global") {
    str = attributes.value("name");
    if (str.isEmpty()) 
      throw Miro::Exception("No item name specified.");
    if (str == "namespace") {
      str = attributes.value("value");
      if (!str.isEmpty())
	generator_.setNamespace(str);
      else 
	throw Miro::Exception("Anonymuous namespace specified.");
    }
    if (str == "include") {
      str = attributes.value("value");
      if (!str.isEmpty())
	generator_.addinclude(str);
      else 
	throw Miro::Exception("Include statement without file name.");
    }
    if (str == "Include") {
      str = attributes.value("value");
      if (!str.isEmpty())
	generator_.addInclude(str);
      else 
	throw Miro::Exception("Include statement without file name.");
    }
  }      


  if (qName == "config_item") {

    class_ = Class();

    // get class name
    str = attributes.value("name");
    if (str.isEmpty())
      throw Miro::Exception("No class name specified.");
    
    parsing_ = true;
    class_.setName(str + "Parameters");
    
    // get parent
    str = attributes.value("parent");
    if (!str.isEmpty()) {
      class_.setParent(str + "Parameters");
    }
    
    // get instance tag
    instance_ = (attributes.value(QString("instance")) == QString("true"));
  }
  else if (parsing_) {
    if (qName == "config_parameter") {
      QString name = attributes.value("name");
      if (name.isEmpty())
	throw Miro::Exception("No parameter name specified.");
      name[0] = name[0].lower();
      QString type = attributes.value("type");
      if (type.isEmpty())
	throw Miro::Exception("No parameter type specified.");
      
      if (type == "string") {
	type = "std::" + type;
	string_ = true;
      }
      if (type == "ACE_Time_Value")
	timeValue_ = true;
      if (type == "ACE_TTY_IO::Serial_Params")
	serialParams_ = true;
      if (type == "ScanDescription")
	scanDescription_ = true;
      
      QString def = attributes.value("default");
      
      str = attributes.value("inherited");
      if (!str.isEmpty() && str == QString("true")) {
	if (def.isEmpty()) {
	  cout << "error: " << str << endl;
	  throw Miro::Exception("Parameter taged inherited without default value.");
	}
	class_.addToConstructor(name + QString(" = ") + def + QString(";"));
      }
      else {
	QString measure = attributes.value("measure");
	
	class_.addParameter(Parameter(type, name, def, measure, QString()));
      }
    }
  }

  return true;
}

bool Parser::endElement(const QString&, const QString&, const QString& qName)
{
  if (parsing_ && qName == "config_item") {
    if (instance_) {
      generator_.addInclude("miro/Singleton.h");
      class_.addStatic(QString("Miro::Singleton<") + 
			    QString(class_.name()) + ">",
			    "instance");
    }
    if (string_)
      generator_.addInclude("string");
    if (timeValue_)
      generator_.addInclude("ace/Time_Value.h");
    if (serialParams_)
      generator_.addInclude("ace/TTY_IO.h");
    if (scanDescription_)
      generator_.addInclude("miro/ScanDescription.h");
    generator_.addClass(class_);

    parsing_ = false;
  }

  return true;
}
