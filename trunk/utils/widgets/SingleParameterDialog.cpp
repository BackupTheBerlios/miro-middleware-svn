// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SingleParameterDialog.h"
#include "SimpleParameter.h"
#include "SimpleParameterEdit.h"
#include "DeferredParameterEdit.h"

#include "params/Parameter.h"

#include "miro/Log.h"

#include <qstring.h>
#include <qhgroupbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include <qscrollview.h>

SingleParameterDialog::
SingleParameterDialog(Miro::CFG::Parameter const& _parameter,
		      QDomNode const& _node,
		      ItemXML * _parentItem,
		      ItemXML * _item,
		      QWidget * _parent, const char * _name) :
  Super(_node.parentNode(), _node,
	_parentItem, _item,
	_parent, _name),
  parameter_(_parameter)
{
  MIRO_ASSERT(!_node.isNull());

  QGridLayout * gridLayout = 
    new QGridLayout(frame_, 1, 3, 2, 5, "gridLayout"); 

  // add parameter struct:
  QLabel * name = new QLabel(frame_);
  QString n = parameter_.name_;
  n[0] = n[0].upper();
  name->setText(n);
  gridLayout->addWidget(name, 0, 0);


  // search existing entry
  QDomNode parameterNode;
  if (!node_.isNull()) {
    parameterNode = node_.firstChild();
    while(!parameterNode.isNull()) {
      QDomElement e = parameterNode.toElement();
      if (!e.isNull() &&
	  e.attribute("name") == n)
	break;
      parameterNode = parameterNode.nextSibling();
    }
  }

  SimpleParameter::Type editType =
    SimpleParameter::typeFromName(parameter_.type_);


  MIRO_ASSERT(editType != SimpleParameter::NONE);

  edit_ = new SimpleParameterEdit(editType,
				  parameter_,
				  parentNode_,
				  node_,
				  parentItem_,
				  item_,
				  frame_,
				  n.latin1());
  
  // add measure
  QLabel * measure = new QLabel(frame_);
  if (!parameter_.measure_.isEmpty()) {
    measure->setText(parameter_.measure_);
    QToolTip::add(measure, (parameter_.type_ != "angle")? parameter_.type_ : QString("double"));
  }
  else
    measure->setText(parameter_.type_);

  gridLayout->addWidget(measure, 0, 2);
  gridLayout->addWidget(edit_->editWidget(), 0, 1);

  frame_->sizeHint();
  groupBox_->sizeHint(); // we need this to get the right size (hutz)
  groupBox_->setTitle("Parameters");
}

void
SingleParameterDialog::setXML()
{
  setModified(false);

  if (edit_->modified()){
    edit_->setXML();
    setModified(edit_->modified());
  }
}

