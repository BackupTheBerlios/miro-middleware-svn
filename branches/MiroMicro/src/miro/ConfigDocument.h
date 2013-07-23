// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
    ConfigDocument(QDomDocument * _document);
    //! Initializing constructor.
    ConfigDocument(int& argc,
                   char * argv[],
                   std::string const& defaultname = std::string(""),
                   std::string const& documentname = std::string("MiroConfigDocument"),
                   StringVector const& userPath = std::vector<std::string>())
    throw(Exception);
    ~ConfigDocument();

    void init(std::string const& _defaultName = std::string(""),
              StringVector const& _userPath = std::vector<std::string>())
    throw(Exception);
    void init(QDomDocument * _document) throw(Exception);

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
  ConfigDocument::setSection(const std::string& _section)
  {
    section_ = _section;
  }
}
#endif // Miro_ConfigDocument_h
