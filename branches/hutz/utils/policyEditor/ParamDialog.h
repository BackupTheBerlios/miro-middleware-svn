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
#ifndef ParamDialog_h
#define ParamDialog_h

#include "PolicyDocument.h"
#include "PolicyConfig.h"

#include <qdialog.h>
#include <qlabel.h>
//#include <qlineedit.h>
#include <qtable.h>
#include <qpushbutton.h>


/**
 * This class shows a dialog for the behaviour parameters.
 */
class ParamDialog : public QDialog
{
  Q_OBJECT

  static const int NUM_LINES = 10;

private:
  QTable* table;
  
  QLabel* labelName;
  QLabel* labelValue;

  QPushButton* cancelButton;
  QPushButton* okButton;
  
  PolicyConfigClass policyConfigClass;

private:
  QString behaviourName;
  QPushButton *getParamButton;

public:
  ParamDialog(QWidget* parent, 
	      const QString& behaviourName, 
	      const PolicyConfigClass& policyConfigClass,
	      QMap<QString, QString> stringMap);
  ~ParamDialog();
 
  QMap<QString, QString> getParameters() const;
};


#endif
