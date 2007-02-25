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
#include "OkCancelDialog.h"
#include "ItemXML.h"

#include "miro/Log.h"

#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qpushbutton.h>

OkCancelDialog::OkCancelDialog(QWidget * _parent, const char * _name, bool _modal) :
  Super(_parent, _name, _modal),
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
OkCancelDialog::accept(bool _accept)
{
  accept_ += (_accept)? -1 : 1;

  MIRO_ASSERT(accept_ >= 0);

  bool a = accept_ == 0;
  emit okay(a);
}

void
OkCancelDialog::accept()
{
  Super::accept();
}
