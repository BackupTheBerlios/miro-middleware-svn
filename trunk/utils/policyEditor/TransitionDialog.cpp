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
#include "TransitionDialog.h"

#include "miro/Log.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qstringlist.h>

TransitionDialog::TransitionDialog(QStringList const& _messages,
				   QStringList const& _targets,
				   QWidget * _parent, const char * _name) :
  Super(_parent, _name)
{
  QGridLayout * gridLayout = 
    new QGridLayout(frame_, 2, 2, 2, 5, "gridLayout"); 
  
  QLabel * msgLabel = new QLabel("Message", frame_);
  gridLayout->addWidget(msgLabel, 0, 0);
  QLabel * targetLabel = new QLabel("Target", frame_);
  gridLayout->addWidget(targetLabel, 1, 0);

  message_ = new QComboBox(frame_);
  message_->insertStringList(_messages);
  gridLayout->addWidget(message_, 0, 1);

  target_ = new QComboBox(frame_);
  target_->insertStringList(_targets);
  gridLayout->addWidget(target_, 1, 1);

  if (_messages.isEmpty() || _targets.isEmpty())
    accept(false);
}

QString
TransitionDialog::getMessage() const 
{
  return message_->currentText();
}

QString
TransitionDialog::getTarget() const 
{
  return target_->currentText();
}
