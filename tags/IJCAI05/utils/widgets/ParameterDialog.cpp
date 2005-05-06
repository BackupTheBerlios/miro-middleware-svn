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

#include "ParameterDialog.h"
#include "SimpleParameter.h"
#include "SimpleParameterEdit.h"
#include "DeferredParameterEdit.h"

#include "ConfigFile.h"

#include "params/Type.h"
#include "params/Generator.h"

#include "miro/Log.h"

#include <qstring.h>
#include <qhgroupbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include <qscrollview.h>
#include <qobjectlist.h>

ParameterDialog::ParameterDialog(Miro::CFG::Type const& _parameterType,
				 QDomNode const& _parentNode, 
				 QDomNode const& _node,
				 ItemXML * _parentItem,
				 ItemXML * _item,
				 QWidget * _parent, const char * _name) :
  Super(_parentNode, _node, _parentItem, _item, _parent, _name),
  config_(ConfigFile::instance()),
  parameterType_(_parameterType),
  params_(config_->description().getFullParameterSet(parameterType_))
{
  MIRO_ASSERT(!_node.isNull());

  initDialog();

  QSize s = frame_->sizeHint();

  if (s.width() > 800 || 
      s.height() > 600) {

    delete frame_;
    QScrollView * sv = new QScrollView(groupBox_, "scrollview");
    frame_ = new QFrame(sv->viewport());
    sv->addChild(frame_);
    sv->setResizePolicy(QScrollView::AutoOneFit);
    
    initDialog();

    frame_->sizeHint();
  }

  groupBox_->sizeHint(); // we seem to need this to get the right size (hutz)
  groupBox_->setTitle("Parameters");
}

void
ParameterDialog::setXML()
{
  setModified(false);
  EditVector::const_iterator first, last = editFelds_.end();
  for (first = editFelds_.begin(); first != last; ++first) {
    if ((*first)->modified()) {
      (*first)->setXML();
      setModified(true);
    }
  }
}

void
ParameterDialog::initDialog()
{
  editFelds_.clear();

  QGridLayout * gridLayout = 
    new QGridLayout(frame_, params_.size(), 3, 2, 5, "gridLayout"); 

  // add parameter structs:
  unsigned long i = 0;
  Miro::CFG::ParameterVector::const_iterator first, last = params_.end();
  for (first = params_.begin(); first != last; ++first, ++i) {

    // name
    QLabel * name = new QLabel(frame_);
    QString n = first->name_;
    n[0] = n[0].upper();
    name->setText(n);
    gridLayout->addWidget(name, i, 0);


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

    // if there is an existing entry
    // and we know our listview,
    // get the corresponding listview item

    ItemXML * childItem = NULL;
    if (!parameterNode.isNull() &&
	item_ != NULL) {
      if (item_->children()) {
	QObjectList childList = *item_->children();
	QObject * c = childList.first();
	while (c) {
	  childItem =
	    dynamic_cast<ItemXML *>(c);
	  MIRO_ASSERT(childItem != NULL);

	  if (childItem->node().toElement().attribute("name") == n) {
	    break;
	  }

	  childItem = NULL;
	  c = childList.next();
	}
      }
    }

    // create the dialog

    ParameterEdit * value;
    SimpleParameter::Type editType =
      SimpleParameter::typeFromName(first->type_);
    if (editType != SimpleParameter::NONE) {
      value = new SimpleParameterEdit(editType,
				      *first,
				      node_,
				      parameterNode,
				      item_,
				      childItem,
				      frame_,
				      n.latin1());

      // add measure
      QLabel * measure = new QLabel(frame_);
      if (!first->measure_.isEmpty()) {
	measure->setText(first->measure_);
	QToolTip::add(measure, (first->type_ != "angle")? first->type_ : QString("double"));
      }
      else
	measure->setText(first->type_);
      gridLayout->addWidget(measure, i, 2);
    }
    else {
      DeferredParameterEdit::EditType deferredEditType =
	DeferredParameterEdit::editType(first->type_);

      value = new DeferredParameterEdit(deferredEditType,
					*first,
					node_,
					parameterNode,
					item_,
					childItem,
					frame_,
					n.latin1());
    }

    gridLayout->addWidget(value->editWidget(), i, 1);
    editFelds_.push_back(value);

  }
}
