// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ConfigDocument.h"
#include "Exception.h"
#include "Parameters.h"
#include "Log.h"

#include <cmath>

#include <ace/Version.h>

 // for string compare
#if ACE_MAJOR_VERSION > 5 || \
    ACE_MAJOR_VERSION == 5 && ACE_MINOR_VERSION >= 4
#include <ace/OS_NS_strings.h>
#else
#include <ace/OS.h>
#endif

#include <ace/Arg_Shifter.h>

#include <qfile.h>
#include <qdom.h>

#include <sstream>

namespace Miro
{

  /** The first existing file that matches the name is returned. */
  std::string
  ConfigDocument::findFile(std::string const& name, 
			   StringVector const& userPath)
  {
    std::string fullName;
    std::vector<std::string> path(userPath);
    std::vector<std::string>::iterator i;

    path.push_back(std::string("."));
    // this should be here, because the plain name could also be right
    // and a . before an absolute path is no longer an absolute path
    path.push_back(std::string("")); 

    if (path.size() == 2) {
      char* miroRoot = ACE_OS::getenv("MIRO_ROOT");
      if (miroRoot) {
	path.push_back(std::string(miroRoot) + std::string("/etc"));
      }
    }

  
    for (i = path.begin(); i != path.end(); ++i) {
      fullName = *i + "/" + name;
      if (QFile::exists(fullName.c_str()))  
	return fullName;
    }
    return std::string();
  }

  ConfigDocument::ConfigDocument() :
    document_(NULL)
  {}

  ConfigDocument::ConfigDocument(QDomDocument * _document) :
    document_(_document)
  {}

  /**
   * The file to use is identified as follows: If a file name
   * argument -MiroConfigFile (or -MCF for short) is given in argc
   * and argv it is used in any case.  If a defaultname != "" is
   * given it is used.  Otherwise the hostname ($HOSTNAME) is
   * exteded by .xml
   */
  ConfigDocument::ConfigDocument(int& argc, 
				 char * argv[], 
				 std::string const& defaultname, 
				 std::string const& documentname,
				 StringVector const& userPath) 
    throw (Exception) :
    document_( new QDomDocument(documentname.c_str()) )
  {
    const char *name;
    char host[256];
    if (defaultname == "") {
      // determine the name of the config file
      ACE_OS::hostname(host, 256);
      name = host;
    } else {
      name = defaultname.c_str();
    }

    ACE_Arg_Shifter arg_shifter (argc, argv);
    while (arg_shifter.is_anything_left ()) {
      const ACE_TCHAR *current_arg = arg_shifter.get_current ();

      const char configFileOpt[] = "-MiroConfigFile";
      const char shortFileOpt[] = "-MCF";
      const int optLen = sizeof(configFileOpt) - 1;
      if ((ACE_OS::strcasecmp(current_arg, configFileOpt) == 0) || 
	  (ACE_OS::strcasecmp(current_arg, shortFileOpt) == 0)) {
	arg_shifter.consume_arg();
	if (arg_shifter.is_parameter_next()) {
	  name = arg_shifter.get_current ();
	  arg_shifter.consume_arg();
	}
      }
      else if (ACE_OS::strncasecmp(current_arg, configFileOpt, optLen) == 0) {
	arg_shifter.consume_arg();
	// The rest of the argument is the ORB id...
        name = current_arg + optLen;
	// but we should skip an optional space...
	if (name[0] == ' ')
	  name++;
      }
      else
	arg_shifter.ignore_arg ();
    }

    std::string fileName = name;
    if (name == host)
      fileName += std::string(".xml");
    std::string fullName = findFile(fileName, userPath );

    if (fullName.length() == 0) {
      MIRO_LOG_OSTR(LL_ERROR, 
		    "ConfigDocument: File not found: " << 
		    fileName << std::endl <<
		    "No config file processing.");
    } 
    else {
      QFile f(fullName.c_str());

      if (!f.open(IO_ReadOnly)) {
	throw CException(errno, "Error opening" + fullName);
      }
      QString parsingError;
      int line = 0;
      int column = 0;
      if (!document_->setContent(&f, &parsingError, &line, &column)) {
	f.close();
	std::stringstream ostr;
	ostr << "error parsing " << fullName << std::endl
	     << " in line " << line << " "
	     << ", column " << column << std::endl
	     << parsingError << std::endl;
	throw Exception(ostr.str());
      }
      f.close();
    }
  }

  ConfigDocument::~ConfigDocument()
  {
    delete document_;
  }

  void ConfigDocument::init(QDomDocument * _document) throw (Exception)
  {
    delete document_;
    document_ = _document;
  }

  void ConfigDocument::init(std::string const& _fileName, 
		       StringVector const& _userPath) 
    throw (Exception)
  {
    delete document_;
    document_ = NULL;

    std::string fullName = findFile(_fileName, _userPath );
    if (fullName.length() == 0) {
      MIRO_LOG_OSTR(LL_ERROR, 
		    "ConfigDocument: File not found: " << 
		    _fileName << std::endl <<
		    "No config file processing.");
    } 
    else {
      QFile f(fullName.c_str());

      if (!f.open(IO_ReadOnly)) {
	throw CException(errno, "Error opening" + fullName);
      }
      QString parsingError;
      int line = 0;
      int column = 0;
      document_ = new QDomDocument("MiroConfigDocument");
      if (!document_->setContent(&f, &parsingError, &line, &column)) {
	f.close();
	std::stringstream ostr;
	ostr << "error parsing " << fullName << std::endl
	     << " in line " << line << " "
	     << ", column " << column << std::endl
	     << parsingError << std::endl;
	throw Exception(ostr.str());
      }
      f.close();
    }
  }

  void
  ConfigDocument::fini() 
  {
    delete document_;
    document_ = NULL;
    section_ = "";
  }

  ConfigDocument::StringVector 
  ConfigDocument::getInstances(const std::string& _type)
  {
    StringVector names;    

    if (document_ != NULL) {
      QString section = section_.c_str();
      QString type = _type.c_str();
      
      // get the root nodes first child
      QDomNode n1 = document_->documentElement().firstChild();
      while(!n1.isNull()) {
	QDomElement e1 = n1.toElement();
	if (!e1.isNull() &&
	    ( (n1.nodeName() == "section" &&
	       e1.attribute("name") == section) ||
	      (n1.nodeName() == section))) {
	  
	  QDomNode n2 = n1.firstChild();
	  while (!n2.isNull()) {
	    QDomElement e2 = n2.toElement();
	    if (!e2.isNull() &&
		n2.nodeName() == "instance" &&
		e2.attribute("type") == type) {
	      std::string name = e2.attribute("name").latin1();
	      if (name != "")
		names.push_back(name);
	    }
	    n2 = n2.nextSibling();
	  }
	}
	n1 = n1.nextSibling();
      }
    }

    return names;
  }

  void 
  ConfigDocument::getParameters(const std::string& _category, 
				ConfigParameters& _parameters)
  {
    unsigned int processed = 0;

    if (document_ != NULL) {
      QString section = section_.c_str();
      QString category = _category.c_str();
      
      // get the root nodes first child
      QDomNode n1 = document_->documentElement().firstChild();
      while(!n1.isNull()) {
	QDomElement e1 = n1.toElement();
	if (!e1.isNull() &&
	    ( (n1.nodeName() == "section" &&
	       e1.attribute("name") == section) ||
	      (n1.nodeName() == section))) {
	  
	  QDomNode n2 = n1.firstChild();
	  while (!n2.isNull()) {
	    QDomElement e2 = n2.toElement();
	    if (!e2.isNull() &&
		( (n2.nodeName() == "parameter" &&
		   e2.attribute("name") == category) ||
		  (n2.nodeName() == category))) {
	      _parameters <<= n2;
	      ++processed;
	    }
	    n2 = n2.nextSibling();
	  }
	}
	n1 = n1.nextSibling();
      }
    }
    if (processed == 0) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: No parameters found for " <<
		    _category << " in section " << section_ <<
		    "\nConfigDocument: Using defaults!");
    }
    else if (processed > 1) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: Multiple parameter sections found for " <<
		    _category << " in section " << section_);
    }
  }

  void 
  ConfigDocument::getType(const std::string& _type, 
			  const std::string& _name, 
			  ConfigParameters& _parameters)
  {
    unsigned int processed = 0;

    if (document_ != NULL) {
      QString section = section_.c_str();
      QString type = _type.c_str();
      QString name = _name.c_str();

      // get the root nodes first child
      QDomNode n1 = document_->documentElement().firstChild();
      while(!n1.isNull()) {
	QDomElement e1 = n1.toElement();
	if (!e1.isNull() &&
	    ( (n1.nodeName() == "section" &&
	       e1.attribute("name") == section) ||
	      (n1.nodeName() == section))) {

	  QDomNode n2 = n1.firstChild();
	  while (!n2.isNull()) {
	    QDomElement e2 = n2.toElement();
	    if (!e2.isNull() &&
		n2.nodeName() == "instance" &&
		e2.attribute("name") == name &&
		e2.attribute("type") == type) {
	      _parameters <<= n2;
	      ++processed;
	    }
	    n2 = n2.nextSibling();
	  }
	}
	n1 = n1.nextSibling();
      }
    }
    if (processed == 0) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: No parameters found for " <<
		    _name << " in section " << section_ <<
		    "\nConfigDocument: Using defaults!");
    }
    else if (processed > 1) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: Multiple parameter sections found for " <<
		    _name << " in section " << section_);
    }
  }

  void 
  ConfigDocument::getInstance(const std::string& _name, 
			      ConfigParameters& _parameters)
  {
    unsigned int processed = 0;

    if (document_ != NULL) {
      QString section = section_.c_str();
      QString name = _name.c_str();

      // get the root nodes first child
      QDomNode n1 = document_->documentElement().firstChild();
      while(!n1.isNull()) {
	QDomElement e1 = n1.toElement();
	if (!e1.isNull() &&
	    ( (n1.nodeName() == "section" &&
	       e1.attribute("name") == section) ||
	      (n1.nodeName() == section))) {

	  QDomNode n2 = n1.firstChild();
	  while (!n2.isNull()) {
	    QDomElement e2 = n2.toElement();
	    if (!e2.isNull() &&
		n2.nodeName() == "instance" &&
		e2.attribute("name") == name) {
	      _parameters <<= n2;
	      ++processed;
	    }
	    n2 = n2.nextSibling();
	  }
	}
	n1 = n1.nextSibling();
      }
    }
    if (processed == 0) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: No parameters found for " <<
		    _name << " in section " << section_ <<
		    "\nConfigDocument: Using defaults!");
    }
    else if (processed > 1) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "ConfigDocument: Multiple parameter sections found for " <<
		    _name << " in section " << section_);
    }
  }
}
