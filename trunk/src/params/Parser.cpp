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

#include "Parser.h"
#include "Generator.h"
#include "Parameter.h"

#include <qstring.h>

namespace Miro
{
  namespace CFG
  {
    Parser::Parser(Generator& _generator) :
      type_(),
      generator_(_generator),
      parsing_(false),
      instance_(false),
      string_(false),
      vector_(false),
      set_(false),
      angle_(false),
      timeValue_(false),
      inetAddr_(false),
      serialParams_(false),
      scanDescription_(false),
      schedParams_(false)
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
      bool rc = false;
      QString str;

      while (true) {
	if (qName == "config") {
	}
	else if (qName == "config_global") {
	  str = attributes.value("name");
	  if (str.isEmpty()) {
	    error_ = "No item name specified.";
	    break;
	  }
	  if (str == "namespace") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addNamespace(str);
	    else {
	      error_ = "Anonymuous namespace specified.";
	      break;
	    }
	  }
	  if (str == "include") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addinclude(str);
	    else {
	      error_ ="Include statement without file name.";
	      break;
	    }
	  }
	  if (str == "Include") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addInclude(str);
	    else {
	      error_ = "Include statement without file name.";
	      break;
	    }
	  }
	  if (str == "src_Include") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addSrcInclude(str);
	    else {
	      error_ = "Source Include statement without file name.";
	      break;
	    }
	  }
	  if (str == "forward") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addForwardDeclaration(str);
	    else {
	      error_ = "Forward declaration statement without name.";
	      break;
	    }
	  }
	  if (str == "local_forward") {
	    str = attributes.value("value");
	    if (!str.isEmpty())
	      generator_.addLocalForwardDeclaration(str);
	    else {
	      error_ = "Local forward declaration statement without name.";
	      break;
	    }
	  }
	}      
	else if (qName == "config_group") {
	  str = attributes.value("name");
	  if (str.isEmpty()) {
	    error_ = "No group name specified.";
	    break;
	  }
	  str[0] = str[0].upper();
	  group_ = str;
	}
	else if (qName == "config_item") {

	  type_ = Type();

	  // get class name
	  str = attributes.value("name");
	  if (str.isNull())
	    str = "";
    
	  parsing_ = true;
	  type_.setName(str);
    
	  // get parent
	  str = attributes.value("parent");
	  if (!str.isEmpty()) {
	    type_.setParent(str + "Parameters");
	  }
    
	  // final attribute
	  str = attributes.value("final");
	  if (!str.isEmpty())
	    type_.setFinal(str != "false");

	  // dummy attribute
	  str = attributes.value("dummy");
	  if (!str.isEmpty())
	    type_.setDummy(str == "true");

	  // dummy attribute
	  str = attributes.value("extern");
	  if (!str.isEmpty())
	    type_.setExtern(str == "true");

	  // get instance tag
	  instance_ = (attributes.value(QString("instance")) == QString("true"));
	}
	else if (parsing_) {
	  if (qName == "config_parameter") {
	    parameterParsing_ = true;

	    QString name = attributes.value("name");
	    if (name.isEmpty()) {
	      error_ = "No parameter name specified.";
	      break;
	    }

	    name[0] = name[0].lower();
	    QString type = attributes.value("type");
	    if (type.isEmpty()) {
	      error_ = "No parameter type specified.";
	      break;
	    }
      
	    if (type == "string") {
	      type = "std::" + type;
	      string_ = true;
	    }
	    else if (type == "std::string") {
	      string_ = true;
	    }
	    else if (type.startsWith("vector<")) {
	      type = "std::" + type;
	      vector_ = true;
	    }
	    else if (type.startsWith("std::vector<")) {
	      vector_ = true;
	    }
	    else if (type.startsWith("set<")) {
	      type = "std::" + type;
	      set_ = true;
	    }
	    else if (type.startsWith("std::set<")) {
	      set_ = true;
	    }
	    else if (type == "Angle") {
	      type = "Miro::" + type;
	    }
	    else if (type == "angle" || type == "Miro::Angle") {
	      angle_ = true;
	    }
	    else if (type == "ACE_Time_Value") {
	      timeValue_ = true;
	    }
	    else if (type == "ACE_INET_Addr") {
	      inetAddr_ = true;
	    }
	    else if (type == "ACE_TTY_IO::Serial_Params") {
	      serialParams_ = true;
	    }
	    else if (type == "ACE_Sched_Params") {
	      schedParams_ = true;
	    }
	    else if (type == "ScanDescriptionIDL") {
	      type = "Miro::" + type;
	      scanDescription_ = true;
	    }
	    else if (type == "Miro::ScanDescriptionIDL") {
	      scanDescription_ = true;
	    }
      
	    QString def = attributes.value("default");
	    if (def.isNull())
	      def = "";

	    QString fullDef;
	    if (type == "ACE_Time_Value" &&
		def.find('.') != -1) {
	      error_ = "ACE_Time_Value format is (x, y), no dot.";
	      break;
	    }
	    else if (type == "std::string")
	      fullDef = "\"" + def + "\"";
	    else if (type == "Miro::Angle")
	      fullDef = "Miro::deg2Rad(" + def + ")";
	    else if (type == "angle")
	      fullDef = "Miro::deg2Rad(" + def + ")";
	    else if (type == "char")
	      fullDef = "'" + def + "'";
	    else if (type == "ACE_INET_Addr")
	      fullDef = "\"" + def + "\"";
	    else
	      fullDef = def;
     
	    str = attributes.value("inherited");
	    if (!str.isEmpty() && str == QString("true")) {
	      if (def.isEmpty())  {
		error_ = "Parameter taged inherited without default value.";
		break;
	      }


	      type_.addToConstructor(name + QString(" = ") + fullDef + QString(";"));
	    }
	    else {
	      QString measure = attributes.value("measure");
	
	      type_.addParameter(Parameter(type, name, def, fullDef, measure, QString()));
	    }
	  }
	  else if (qName == "constructor") {
	    ctorParsing_ = true;
	    ctor_ = "";
	  }
	  else if (qName == "documentation") {
	    docuParsing_ = true;
	    docu_ = "";
	  }
	  else {
	    error_ = "Unknown tag name: " + qName;
	    break;
	  }
	}
	else {
	  error_ = "Unknown tag name: " + qName;
	  break;
	}

	rc = true;
	break;
      }
      return rc;
    }

    bool 
    Parser::endElement(const QString&, const QString&, const QString& qName)
    {
      bool rc = false;
      while(true) {
	if (parsing_ && qName == "config_item") {
	  if (type_.isExtern() && !type_.parent().isEmpty()) {
	    error_ = "Type " + type_.name() + " defined extern but also has a parent.";
	    break;
	  }
	  if (instance_) {
	    generator_.addInclude("miro/Singleton.h");
	    type_.setInstance();
	  }
	  if (string_)
	    generator_.addInclude("string");
	  if (vector_) {
	    generator_.addInclude("vector");
	    generator_.addSrcInclude("miro/IO.h");
	  }
	  if (set_) {
	    generator_.addInclude("set");
	    generator_.addSrcInclude("miro/IO.h");
	  }
	  if (angle_)
	    generator_.addInclude("miro/Angle.h");
	  if (timeValue_) {
	    generator_.addInclude("ace/Time_Value.h");
	    generator_.addSrcInclude("miro/TimeHelper.h");
	  }
	  if (serialParams_) {
	    generator_.addInclude("ace/TTY_IO.h");
	    generator_.addSrcInclude("miro/IO.h");
	  }
	  if (schedParams_) {
	    generator_.addInclude("ace/Sched_Params.h");
	    generator_.addSrcInclude("miro/IO.h");
	  }
	  if (instance_) {
	    generator_.addInclude("ace/INET_Addr.h");
	    generator_.addSrcInclude("miro/IO.h");
	  }
	  if (scanDescription_) {
	    generator_.addInclude("idl/ScanDescriptionC.h");
	    generator_.addSrcInclude("idl/RangeSensorC.h");
	    generator_.addSrcInclude("miro/ScanDescriptionHelper.h");
	  }
	  generator_.addType(group_, type_);

	  parsing_ = false;
	}
	else if (parameterParsing_ && qName == "config_parameter") {
	  parameterParsing_ = false;
	}
	else if (ctorParsing_ && qName == "constructor") {
	  ctorParsing_ = false;
	  type_.addToConstructor(ctor_);
	}
	else if (docuParsing_ && qName == "documentation") {
	  docuParsing_ = false;
	}

	rc = true;
	break;
      }
      return rc;
    }

    QString
    Parser::errorString() {
      return error_;
    }

    bool
    Parser::characters(const QString& ch)
    {
      if (ctorParsing_) {
	ctor_ += ch;
      }
      return true;
    }
  }
}
