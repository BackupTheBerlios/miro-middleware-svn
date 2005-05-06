// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "EventTypeDialog.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

EventTypeDialog::EventTypeDialog(CosNotification::EventType const& _eventType,
				 QWidget * _parent, const char * _name) :
  Super(_parent, _name)
{
  setCaption("Event type");

  QVBoxLayout * topBox = new QVBoxLayout(this, 0, -1, "boxLayout");

  QWidget * dummy = new QWidget(this, "dummy");

  QGridLayout * inputGrid = new QGridLayout(dummy, 2, 2, 0, -1, "grid");

  QLabel * domainLabel = new QLabel("Domain name ", dummy, "domainLabel");
  QLabel * typeLabel = new QLabel("Type name ", dummy, "typeLabel");
  domainName_ = new QLineEdit((char const * const) _eventType.domain_name, dummy, "domainInput");
  typeName_ = new QLineEdit((char const * const) _eventType.type_name, dummy, "typeInput");

  inputGrid->addWidget(domainLabel, 0, 0);
  inputGrid->addWidget(typeLabel, 1, 0);
  inputGrid->addWidget(domainName_, 0, 1);
  inputGrid->addWidget(typeName_, 1, 1);

  topBox->addSpacing(10);
  topBox->addWidget(dummy);

  topBox->addSpacing(10);
  QHBoxLayout * dialogButtonsBox = new QHBoxLayout(topBox, -1, "hBoxLayout");
  QSpacerItem * dBSpace = new QSpacerItem(0, 0);
  QPushButton * okButton = new QPushButton("OK", this);
  QPushButton * cancelButton = new QPushButton("Cancel", this);
  
  topBox->addSpacing(5);
  dialogButtonsBox->addItem(dBSpace);
  dialogButtonsBox->addWidget(okButton);
  dialogButtonsBox->addSpacing(5);
  dialogButtonsBox->addWidget(cancelButton);
  dialogButtonsBox->addSpacing(5);
  
  okButton->setDefault(true);

  // connect the dialogs functionality  
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

CosNotification::EventType
EventTypeDialog::result() const
{
  CosNotification::EventType eventType;

  eventType.domain_name = CORBA::string_dup(domainName_->text());
  eventType.type_name = CORBA::string_dup(typeName_->text());

  return eventType;
}
