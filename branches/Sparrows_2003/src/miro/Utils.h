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

#include <string>
#include <vector>

#include <termios.h>

// forward declarations
class QDomDocument;

namespace Miro 
{
  // forward declarations
  class ConfigParameters;
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
    ~ConfigDocument();    

    void setSection(const std::string& _section);
    void getParameters(const std::string& _category, ConfigParameters& parameters);

  protected:
    QDomDocument * document_;

    std::string section_;
  };

inline
void 
ConfigDocument::setSection(const std::string& _section) {
  section_ = _section;
}

};
#endif




