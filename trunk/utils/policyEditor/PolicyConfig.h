// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PolicyConfig_h
#define PolicyConfig_h

#include <miro/Singleton.h>

#include <qstring.h>
#include <qstringlist.h>

// forward declarations
class Generator;
class Parser;

class PolicyConfigClass
{
public:
  //! Initializing constructor.
  PolicyConfigClass();
  //! Destructor
  ~PolicyConfigClass();
  
  void setDescriptionFiles(const QStringList& _files);
  const QStringList& getDescriptionFiles() const;
  
  static Miro::Singleton<PolicyConfigClass> instance;
  
  const Generator& description() const;

protected:
  void readConfigFile();
  void writeConfigFile();
  void parseDescriptionFiles();

  //! fully qualified name of the config file
  QString configFile_;
  //! list of description files 
  QStringList descriptionFiles_;

  Generator * generator_;
  Parser * handler_;
};

inline
const QStringList&
PolicyConfigClass::getDescriptionFiles() const {
  return descriptionFiles_;
}

inline
const Generator&
PolicyConfigClass::description() const {
  return *generator_;
}

#endif
