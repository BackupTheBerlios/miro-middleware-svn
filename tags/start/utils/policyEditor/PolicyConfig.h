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

#include "BehaviourDescription.h"
#include "BehaviourParam.h"

#include <qdom.h>
#include <qstring.h>
#include <qwidget.h>

#include <vector>

class PolicyConfigClass
{
public:
  PolicyConfigClass();
  
private:
  /**contains the path of the config-file*/
  QString configFile;
  QString behaviourFile;

  /**the main document structure containing the config-Params*/
  QDomDocument domDocument_behaviours;
  QDomDocument domDocument_config;
  
public:
  //**returns all behaviours that are known*/
  
  std::vector<BehaviourDescription> getBehaviours() const;
  std::vector<QString> getArbiters() const;
  std::vector<BehaviourParam> getBehaviourParams(const QString& behaviourName) const;
  
  void setNewBehaviourDescriptionFileName(const QString& file);
  
  QString getBehaviourDescriptionFileName() const;
  void setBehaviourDescriptionFileName();
  
  void getBehaviourDescription();  //reads the behaviour-Description-File
  
  void setConfiguration(QWidget *parent);
};

#endif
