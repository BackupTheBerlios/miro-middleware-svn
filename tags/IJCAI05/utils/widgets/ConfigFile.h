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
#ifndef ConfigFile_h
#define ConfigFile_h

#include <miro/Singleton.h>

#include <qstring.h>
#include <qstringlist.h>

// forward declarations
namespace Miro {
  namespace CFG {
    class Generator;
    class Parser;
  }
}

class ConfigFileName
{
public:
  ConfigFileName();

  QString const & fileName() const;
  void setFileName(QString const& _name);

  static Miro::Singleton<ConfigFileName> instance;

protected:
  QString fileName_;

private:
  //  ~ConfigFileName();
};

inline
QString const&
ConfigFileName::fileName() const {
  return fileName_;
}

inline
void
ConfigFileName::setFileName(QString const& _name) {
  fileName_ = _name;
}


class ConfigFile
{
public:
  //! Initializing constructor.
  ConfigFile();
  //! Destructor
  ~ConfigFile();
  
  void setDescriptionFiles(const QStringList& _files);
  const QStringList& getDescriptionFiles() const;
  
  static Miro::Singleton<ConfigFile> instance;
  
  const Miro::CFG::Generator& description() const;

protected:
  void readConfigFile();
  void writeConfigFile();
  void parseDescriptionFiles();

  //! fully qualified name of the config file
  QString configFile_;
  //! list of description files 
  QStringList descriptionFiles_;

  Miro::CFG::Generator * generator_;
  Miro::CFG::Parser * handler_;

private:
};

inline
const QStringList&
ConfigFile::getDescriptionFiles() const {
  return descriptionFiles_;
}

inline
const Miro::CFG::Generator&
ConfigFile::description() const {
  return *generator_;
}

#endif
