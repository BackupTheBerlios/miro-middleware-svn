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

#include "PolicyConfigurationDialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent, 
					 PolicyConfigClass* policyConfigClass): 
  QDialog(parent, "Configuration-Settings", TRUE)
{
  this->policyConfigClass=policyConfigClass;
  this->resize(350,550);
   
  comboBoxBehaviours=new QComboBox(true, this, "behaviourComboBox");
  comboBoxBehaviours->setGeometry(10,10,200,30);
   
  vectorBehaviourDescription=this->policyConfigClass->getBehaviours();
  for (unsigned int i=0; i<vectorBehaviourDescription.size(); i++)
  {
    comboBoxBehaviours->insertItem(vectorBehaviourDescription[i].getName(),-1);
  }
   
  tableBehaviours=new QTable(10,2,this,"tableBehavours");
  tableBehaviours->setGeometry(10,50,300,400);
  
  
  connect(comboBoxBehaviours,SIGNAL(activated(QString)),this,SLOT(getActivatedBehaviour(QString)));
   
  buttonOK=new QPushButton("OK",this);
  buttonOK->setGeometry(10, 475,100,30);
  connect(buttonOK, SIGNAL(clicked()), SLOT(accept()));
   
   
}

void ConfigurationDialog::getActivatedBehaviour(QString selectedBehaviour)
{
  vectorBehaviourParams=policyConfigClass->getBehaviourParams(selectedBehaviour);
  tableBehaviours->setNumRows(vectorBehaviourParams.size());
  for (unsigned int i=0; i<vectorBehaviourParams.size();i++)
  {
    tableBehaviours->setText(i,0,vectorBehaviourParams[i].getName());
    tableBehaviours->setText(i,1,vectorBehaviourParams[i].getType());
  }
  tableBehaviours->adjustColumn(0);
  tableBehaviours->adjustColumn(1);
}
