// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_ConfigDocument_h
#define Miro_ConfigDocument_h

#include "Exception.h"

#include <string>
#include <vector>

// forward declarations
class QDomDocument;

namespace Miro 
{
  // forward declarations
  class ConfigParameters;

  class ConfigDocument 
  {
  public:
    typedef std::vector<std::string> StringVector;

    //! Default constructor.
    ConfigDocument();
    //! Initializing constructor.
    ConfigDocument(int& argc, 
		   char * argv[], 
		   std::string const& defaultname = std::string(""), 
		   std::string const& documentname = std::string("MiroConfigDocument"),
		   StringVector const& userPath = std::vector<std::string>())
      throw (Exception);
    ~ConfigDocument();    

    void init(std::string const& _defaultName = std::string(""), 
	      StringVector const& _userPath = std::vector<std::string>())
      throw(Exception);

    void fini();

    void setSection(std::string const& _section);

    StringVector getInstances(std::string const& _type);

    void getInstance(std::string const& _name, 
		     ConfigParameters& parameters);

    void getType(std::string const& _type,
		 std::string const& _name, 
		 ConfigParameters& parameters);

    void getParameters(std::string const& _name, 
		       ConfigParameters& parameters);


    //! Create the fully qualified path from a file name and a set of search paths.
    static std::string 
    findFile(std::string const& name, 
	     std::vector<std::string> const& userPath);


  protected:
    QDomDocument * document_;

    std::string section_;
  };

  inline
  void 
  ConfigDocument::setSection(const std::string& _section) {
    section_ = _section;
  }
}
#endif // Miro_ConfigDocument_h
