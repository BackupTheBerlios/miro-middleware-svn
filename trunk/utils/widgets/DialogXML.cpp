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

#include "DialogXML.h"
#include "ItemXML.h"

#include "miro/Log.h"

#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qpushbutton.h>

DialogXML::DialogXML(QDomNode const& _parentNode, QDomNode const& _node,
		     ItemXML * _parentItem, ItemXML * _item,
		     QWidget * _parent, const char * _name, bool _modal) :
  Super(_parent, _name, _modal),
  parentNode_(_parentNode),
  node_(_node),
  parentItem_(_parentItem),
  item_(_item),
  frame_(NULL),
  modified_(false),
  accept_(0)
{
  QString n = _name;
  n[0] = n[0].upper();
  setCaption(n);

  QVBoxLayout * topBox = new QVBoxLayout(this, 0, -1, "boxLayout");

  groupBox_ = new QVGroupBox(this, "groupbox");
  frame_ = new QFrame(groupBox_, "parameterframe");

  QHBoxLayout * dialogButtonsBox = new QHBoxLayout(NULL, 0, -1, "hBoxLayout");
  QSpacerItem * dBSpace = new QSpacerItem(0, 0);
  QPushButton * okButton = new QPushButton("OK", this);
  QPushButton * cancelButton = new QPushButton("Cancel", this);

  topBox->addSpacing(10);
  topBox->addWidget(groupBox_);
  topBox->addSpacing(10);
  topBox->addLayout(dialogButtonsBox);
  topBox->addSpacing(5);

  dialogButtonsBox->addItem(dBSpace);
  dialogButtonsBox->addWidget(okButton);
  dialogButtonsBox->addSpacing(5);
  dialogButtonsBox->addWidget(cancelButton);
  dialogButtonsBox->addSpacing(5);
  
  okButton->setDefault(true);

  // connect the dialogs functionality  
  connect(this,         SIGNAL(okay(bool)), okButton, SLOT(setEnabled(bool)));
  connect(okButton,     SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

void
DialogXML::accept(bool _accept)
{
  accept_ += (_accept)? -1 : 1;

  MIRO_ASSERT(accept_ >= 0);

  bool a = accept_ == 0;
  emit okay(a);
}
