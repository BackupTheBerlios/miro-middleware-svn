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
#ifndef PolicyConfiggurationDialog_h
#define PolicyConfigurationDialog_h

#include "BehaviourDescription.h"
#include "BehaviourParam.h"
#include "PolicyConfig.h"

#include <qcombobox.h>
#include <qtable.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qdialog.h>
#include <qwidget.h>

#include <vector>

class ConfigurationDialog: public QDialog
{
  Q_OBJECT
 
private:
  QComboBox* comboBoxBehaviours;
  QTable* tableBehaviours;
  QPushButton* buttonOK;
  std::vector<BehaviourDescription> vectorBehaviourDescription;
  std::vector<BehaviourParam> vectorBehaviourParams;
  PolicyConfigClass* policyConfigClass ;
  
public:
  ConfigurationDialog(QWidget* parent, PolicyConfigClass* policyConfigClass); 
  
  
private slots:
  void getActivatedBehaviour(QString selectedBhaviour);
 
};

#endif
