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


#include "Parser.h"
#include "Generator.h"
#include "Parameter.h"

#include <qstring.h>

#include <string>
#include <iostream>

bool Parser::startDocument()
{
  return true;
}

bool Parser::startElement( const QString&, const QString&, const QString& qName, 
			   const QXmlAttributes& attributes)
{
  if (qName == "config_item") {

    // reset internal state
    string_ = false;
    timeValue_ = false;
    scanDescription_ = false;

    generator_ = new Generator();

    QString str;

    // get namespace
    str = attributes.value("namespace");
    if (!str.isEmpty())
      generator_->setNamespace(str);
      
    // get class name
    str = attributes.value("name");
    if (str.isEmpty())
      throw std::string("No class name specified.");

    generator_->setName(str);
    generator_->setFileName(str);

    // get parent
    str = attributes.value("parent");
    if (!str.isEmpty()) {
      generator_->setParent(str);

      str.append(".hh");
      int pos = str.find("::");
      if (pos == -1)
	generator_->addinclude(str);
      else {
	str[0] = str[0].lower();
	str.replace(pos, 2, "/");
	generator_->addInclude(str);
      }
    }

    // get instance tag
    instance_ = (attributes.value(QString("instance")) == QString("true"));
  }

  else if (qName == "config_parameter") {
    QString name = attributes.value("name");
    if (name.isEmpty())
      throw string("No parameter name specified.");
    QString type = attributes.value("type");
    if (type.isEmpty())
      throw string("No parameter type specified.");

    if (type == "string")
      string_ = true;
    if (type == "ACE_Time_Value")
      timeValue_ = true;
    if (type == "ScanDescription")
      scanDescription_ = true;

    QString def = attributes.value("default");

    if (!attributes.value(QString("inherited")) == QString("true")) {
      if (def.isEmpty())
	throw string("Parameter taged inherited without default value.");
      generator_->addToConstructor(name + QString(" = ") + def + QString(";"));
    }
    else {
      QString measure = attributes.value("measure");

      generator_->addParameter(Parameter(type, name, def, measure, QString()));
    }
  }

  return true;
}

bool Parser::endElement( const QString&, const QString&, const QString& qName)
{
  if (qName == "config_item") {

    if (instance_) {
      generator_->addInclude("miro/singleton.hh");
      generator_->addStatic(QString("Miro::Singleton<") + QString(generator_->name()) + ">",
			   "instance");
    }
    if (string_)
      generator_->addInclude("string");
    if (timeValue_)
      generator_->addInclude("ace/Time_Value.h");
    if (scanDescription_)
      generator_->addInclude("miro/scanDescription.hh");

    generator_->generateHeader();
    cout << endl << endl << endl;
    generator_->generateSource();
    cout << endl << endl << endl;
  }

  return true;
}
