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

#include "ParamDialog.h"
#include "PolicyConfig.h"

#include <qmessagebox.h>

ParamDialog::ParamDialog(QWidget* parent,
			 const QString& behaviourName, 
			 const PolicyConfigClass& policyConfigClass,
			 QMap<QString, QString> stringMap) :
  QDialog(parent, "ParamDialog", TRUE)       // TRUE = modal dialog
{
  this->behaviourName = behaviourName;
  this->policyConfigClass = policyConfigClass;
  resize(400, 500);
  
  labelName = new QLabel("Name", this);
  labelName->setGeometry(10, 10, 150, 30);

  labelValue = new QLabel("Value", this);
  labelValue->setGeometry(170, 10, 100, 30);
  
  vector<BehaviourParam> vectorBehaviourParams = policyConfigClass.getBehaviourParams(behaviourName);
  
  table = new QTable(vectorBehaviourParams.size(), 4, this);
  table->setGeometry(10, 50, 350, 400);
  
  // fill table with current parameters
  QMap<QString, QString>::Iterator iter;
  //iter=stringMap.begin();
  for (unsigned int i=0; i < vectorBehaviourParams.size(); ++i)
  {
    table->setText(i,0,vectorBehaviourParams[i].getName());
    iter = stringMap.find(vectorBehaviourParams[i].getName());
    if (iter!=stringMap.end())
    {
      table->setText(i,1,iter.data());
    }
    table->setText(i,2,vectorBehaviourParams[i].getType());
    table->setText(i,3,vectorBehaviourParams[i].getMeasure());
  }
  table->adjustColumn(0);
  table->adjustColumn(2);
  table->adjustColumn(3);

  cancelButton = new QPushButton("Cancel", this);
  cancelButton->setGeometry(10, 460, 100, 30);

  okButton = new QPushButton("OK", this);
  okButton->setGeometry(120, 460, 100, 30);
  
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

ParamDialog::~ParamDialog()
{
  delete okButton;
  delete cancelButton;
  //delete getParamButton;

  delete labelName;
  delete labelValue;
  delete table;
}


QMap<QString, QString>
ParamDialog::getParameters() const
{
  bool occuredTypemissmatch=false;
  bool typemissmatch=false;
  bool ok=true; 
  QMap<QString, QString> stringMap;

  for (int i=0; i<table->numRows(); i++) {
    QString key   = table->text(i,0);
    QString value = table->text(i,1);

    if ((!value.isEmpty())&&(!key.isEmpty())) 
    {
      //type-check:
      QString type=table->text(i,2);
      if (type.compare("int")==0)
      {
        value.toInt(&ok,10);
	if (!ok) typemissmatch=true;
      }
      if (type.compare("double")==0)
      {
        value.toDouble(&ok);
	if (!ok) typemissmatch=true;
      }
      if (type.compare("bool")==0)
      { 
        if ((value.compare("true")!=0) && (value.compare("false")!=0))
	{
	   typemissmatch=true;
	 }
       }
       if (!typemissmatch) stringMap.insert(key, value);
       if (typemissmatch) {
       	 occuredTypemissmatch=true;
	 typemissmatch=false;
       }
     }
  }
  if (occuredTypemissmatch)
  {
    cout <<"One or more type-missmatches occured" <<endl; 
    QMessageBox::information((QWidget *) this, "Behaviour-Setup", "Type-missmatch occured");
  }
  return stringMap;
}
