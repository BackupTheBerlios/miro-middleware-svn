// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
