// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SimpleParameterEdit.h"
#include "ParameterDialog.h"
#include "Validators.h"

#include "params/Parameter.h"

#include "miro/Log.h"

#include <qstring.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <qlistview.h>

#include <climits>

SimpleParameterEdit::SimpleParameterEdit(SimpleParameter::Type _type,
					 Miro::CFG::Parameter const& _parameter,
					 QDomNode& _parentNode, 
					 QDomNode& _node,
					 ItemXML * _parentItem, 
					 ItemXML * _item,
					 QWidget * _parent, 
					 const char * _name) :
  Super(_parameter, _parentNode, _node, 
	_parentItem, _item,
	_parent, _name),
  type_(_type),
  lineEdit_(new QLineEdit(_parent, "line_edit"))
{
  editWidget_ = lineEdit_;

  MIRO_ASSERT(!parentNode_.isNull());

  // customize lineEdit for typesafe editing
  QValidator *  v = NULL;
  switch (_type) {
  case SimpleParameter::BOOL:
    v = new MyBoolValidator(this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::CHAR:
    lineEdit_->setMaxLength(1);
    break;
  case SimpleParameter::SHORT:
    v = new MyIntValidator(SHRT_MIN, SHRT_MAX, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::USHORT:
    v = new MyIntValidator(0, USHRT_MAX, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::INT:
    v = new MyIntValidator(INT_MIN + 1, INT_MAX - 1, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::UINT:
    v = new MyIntValidator(0, INT_MAX - 1, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::LONG:
    v = new MyIntValidator(LONG_MIN + 1, LONG_MAX - 1, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::ULONG:
    v = new MyIntValidator(0, LONG_MAX - 1, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::DOUBLE:
    v = new MyDoubleValidator(this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::ANGLE:
    v = new MyDoubleValidator(this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::MIRO_ANGLE:
    v = new MyDoubleValidator(-180., 180., 6, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::STRING:
    break;
  case SimpleParameter::ACE_TIME_VALUE:
    v = new MyDoubleValidator(0, ULONG_MAX, 6, this);
    lineEdit_->setValidator(v);
    break;
  case SimpleParameter::ACE_INET_ADDR:
    break;
  default:
    break;
  }

  // connect validator, if we have one
  if (v) {
    QObject * p = _parent;
    while (p != NULL) {
      if (dynamic_cast<ParameterDialog *>(p) != NULL) {
	connect(v, SIGNAL(acceptable(bool)), p, SLOT(accept(bool)));
	break;
      }
      p = p->parent();
    }
  }

  // add default as tooltip
  if (parameter_.default_)
    QToolTip::add(lineEdit_, QString("default: ") + parameter_.default_);

  // set current value
  if (!node_.isNull()) {
    QDomElement e = node_.toElement();
    if (!e.isNull() && e.hasAttribute("value")) {
      lineEdit_->setText(e.attribute("value"));
    }
  }

  // set lineEdit to unedited
  lineEdit_->setEdited(false);
}

void
SimpleParameterEdit::setXML()
{
  // no edit -> nothing to be done
  if (!lineEdit_->edited())
    return;

  // delete entry if edit field is empty
  if (!parentNode_.isNull() &&
      ( lineEdit_->text().isEmpty())) {
    if (!node_.isNull()) {
      // if we know about the associated list view,
      // we delete this one too
      if (item_ != NULL) {
	delete item_;
      }
      parentNode_.removeChild(node_);
    }
    else
      lineEdit_->setEdited(false);
    return;
  }  

  // crete a node if necessary
  if (!parentNode_.isNull() &&
      node_.isNull()) {

    MIRO_ASSERT(!parentNode_.ownerDocument().isNull());
    QDomElement e = parentNode_.ownerDocument().createElement(XML_TAG_PARAMETER);

    e.setAttribute(XML_ATTRIBUTE_KEY, name());
    node_ = parentNode_.appendChild(e);

    MIRO_ASSERT(!node_.isNull());
    
    // if we know about the associated list view,
    // we add the edit field, too
    if (parentItem_ != NULL) {
      ParameterXML * newParam =
	new SimpleParameter(parameter_,
			    node_,
			    parentItem_->listViewItem(), NULL,
			    parentItem_, name());
      newParam->init();
      item_ = newParam;
    }
  }

  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());

  if (e.attribute(XML_ATTRIBUTE_VALUE) != lineEdit_->text()) {
    e.setAttribute(XML_ATTRIBUTE_VALUE, lineEdit_->text());
    // if we know about the associated list view,
    // we set the new value there, too
    if (item_ != NULL) {
      item_->listViewItem()->setText(1, e.attribute(XML_ATTRIBUTE_VALUE));
    }
  }
  else {
    lineEdit_->setEdited(false);
  }
}

bool
SimpleParameterEdit::modified() const 
{
  return lineEdit_->edited();
}
