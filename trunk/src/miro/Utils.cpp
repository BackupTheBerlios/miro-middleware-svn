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
#include "miro/DevParameters.h"

#include <ace/Arg_Shifter.h>

#include <qfile.h>

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <stdlib.h> // getenv ...
#include <sys/stat.h>
#include <sys/types.h>

#undef DEBUG

using std::string;
using std::cout;
using std::cerr;
using std::vector;

namespace Miro
{
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
    document( documentname.c_str() )
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

      if (!f.open(IO_ReadOnly))
	throw CException(errno, std::strerror(errno));
      if (!document.setContent(&f)) {
	f.close();
	throw CException(errno, std::strerror(errno));
      }
      f.close();
    }
  }

  void 
  ConfigDocument::setRobotType(const char name[])
  {
    QDomElement docElem = document.documentElement();
    sections = document.elementsByTagName(name);
  }
  void 
  ConfigDocument::setSection(const char name[])
  {
    QDomElement docElem = document.documentElement();
    sections = document.elementsByTagName(name);
  }

  void 
  ConfigDocument::getParameters(const char category[], 
				Parameters& parameters)
  {
    //cerr << "get params" << endl;
    for (unsigned int i = 0; i < sections.length(); ++i) {
      //cerr << "doing section " << i << endl;
      QDomNode n = sections.item(i).firstChild();
      //cerr << "after first child" << endl;
      while(!n.isNull()) {
	if (n.nodeName() == QString(category)) {
	  parameters <<= n;
	  //cerr << "after <<=" << endl;
	}
	n = n.nextSibling();
	//cerr << "after next" << endl;
      }
    }
  }
};

//
// Template instantiations
//
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class std::vector<std::string>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate std::vector<std::string>

#endif
