// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Utils.h"
#include "miro/Exception.h"
#include "miro/Parameters.h"

#include <ace/Arg_Shifter.h>

#include <qfile.h>
#include <qdom.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <unistd.h>
#include <stdlib.h> // getenv ...
#include <sys/stat.h>
#include <sys/types.h>

#undef DEBUG

namespace Miro
{
  using std::string;
  using std::cout;
  using std::cerr;
  using std::vector;

  string
  findFile(const string& name, const vector<string>& userPath)
  {
    struct stat statBuf;

    string fullName;
    vector<string> path(userPath);
    vector<string>::iterator i;

    path.push_back(string("."));
    char* miroRoot = ACE_OS::getenv("MIRO_ROOT");
    if (miroRoot) {
      path.push_back(string(miroRoot) + string("/etc"));
    }
    // this should be here, because the plain name could also be right
    // and a . before an absolute path is no longer an absolute path
    path.push_back(string("")); 
  
    for (i = path.begin(); i != path.end(); ++i) {
      fullName = *i + "/" + name;
      if (stat(fullName.c_str(), &statBuf) == 0)  
	return fullName;
    }
    return string();
  }

  ConfigDocument::ConfigDocument(int& argc, 
				 char * argv[], 
				 const std::string& defaultname, 
				 const std::string& documentname,
				 const std::vector<std::string> &userPath ) :
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
//      char *current_arg = arg_shifter.get_current ();
      const ACE_TCHAR *current_arg = arg_shifter.get_current ();

      const char configFileOpt[] = "-MiroConfigFile";
      const int optLen = sizeof(configFileOpt) - 1;
      if (ACE_OS::strcasecmp(current_arg, configFileOpt) == 0) {
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

    string fileName = name;
    if (name == host)
      fileName += std::string(".xml");
    string fullName = Miro::findFile(fileName, userPath );

    if (fullName.length() == 0) {
      cerr << "File not found: " << fileName << endl;
      cerr << "No config file processing." << endl;
    } 
    else {
      QFile f(fullName.c_str());

      if (!f.open(IO_ReadOnly)) {
	cout << "error on open" << endl;
	throw CException(errno, std::strerror(errno));
      }
      QString parsingError;
      int line;
      int column;
      if (!document_->setContent(&f, &parsingError, &line, &column)) {
	f.close();
	std::stringstream ostr;
	ostr << "error parsing " << fullName << endl
	     << " in line " << line << " "
	     << ", column " << column << endl
	     << parsingError << endl;
	throw Exception(ostr.str());
      }
      f.close();
    }
  }

  ConfigDocument::~ConfigDocument()
  {
    delete document_;
  }

  void 
  ConfigDocument::getParameters(const std::string& _category, 
				ConfigParameters& parameters)
  {
    QString section = section_.c_str();
    QString category = _category.c_str();

    // get the root nodes first child
    QDomNode n1 = document_->documentElement().firstChild();
    while(!n1.isNull()) {
      QDomElement e1 = n1.toElement();
      if (!e1.isNull() &&
	  (n1.nodeName() == "section" &&
	   e1.attribute("name") == section) ||
	  (n1.nodeName() == section)) {

	QDomNode n2 = n1.firstChild();
	while (!n2.isNull()) {
	  QDomElement e2 = n2.toElement();
	  if (!e2.isNull() &&
	      (n2.nodeName() == "parameter" &&
	       e2.attribute("name") == category) ||
	      (n2.nodeName() == category)) {
	    parameters <<= n2;
	  }
	  n2 = n2.nextSibling();
	}
      }
      n1 = n1.nextSibling();
    }
  }
};
