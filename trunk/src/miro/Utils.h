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
#ifndef miroUtils_hh
#define miroUtils_hh

#include <ace/OS.h>

#include <qdom.h>

#include <string>
#include <vector>

#include <termios.h>

namespace Miro 
{
  // forward declarations
  class Parameters;
  //  class DevParameters;

  std::string 
  Miro::findFile(const std::string& name, const std::vector<std::string>& userPath);

  class ConfigDocument 
  {
  public:
    /**
     * the file to use is identified as follows:
     * if an argument if given in argc and argv it is used in any case
     * else if a defaultname != "" is given it is used
     * else the hostname is extneded by .xml
     */
    ConfigDocument(int& argc, 
		   char * argv[], 
		   const std::string& defaultname = std::string(""), 
		   const std::string& documentname = std::string("MiroConfigDocument"),
		   const std::vector<std::string> &userPath = std::vector<std::string>() );
    
    void setRobotType(const char name[]);
    void setSection(const char name[]);
    void getParameters(const char category[], Parameters& parameters);

  protected:
    QDomDocument document;
    QDomNodeList sections;
  };
};

#endif




