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

#include "DeferredParameterEdit.h"
#include "ParameterDialog.h"
#include "ParameterListDialog.h"
#include "CompoundParameter.h"

#include "params/Parameter.h"
#include "params/Generator.h"
#include "utils/widgets/ConfigFile.h"

#include "miro/Log.h"

#include <qpushbutton.h>
#include <qstring.h>
#include <qlistview.h>

namespace 
{
  struct Pair {
    char const * name;
    DeferredParameterEdit::EditType type;
  };

  Pair typeMap [2] = {
    { "std::vector<", DeferredParameterEdit::VECTOR },
    { "std::set<",    DeferredParameterEdit::SET }
  };
}


DeferredParameterEdit::EditType 
DeferredParameterEdit::editType(QString const& _type)
{
  for (unsigned int i = 0; i < 2; ++i) {
    if (_type.startsWith(typeMap[i].name))
      return typeMap[i].type;
  }
  return DeferredParameterEdit::NESTED_PARAMETER;
}

DeferredParameterEdit::DeferredParameterEdit(EditType _type,
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
  modified_(false),
  tmpDocument_("MiroConfigDocument")
{
  editWidget_ = new QPushButton(_parameter.type_, _parent, "edit button");
  connect(editWidget_, SIGNAL(clicked()), this, SLOT(deferredEdit()));

  // create a copy of the parameters xml tree
  tmpParentNode_ = tmpDocument_.createElement("section");
  tmpDocument_.appendChild(tmpParentNode_);

  QDomElement e = tmpDocument_.createElement(XML_TAG_PARAMETER);
  e.setAttribute(XML_ATTRIBUTE_KEY, name());
  tmpNode_ = e;
  if (!node_.isNull())
    tmpNode_ = node_.cloneNode();
  tmpParentNode_.appendChild(tmpNode_);
}

void
DeferredParameterEdit::deferredEdit()
{
  DialogXML * dialog = NULL;

  if (type_ == NESTED_PARAMETER) {
    QString typeName = parameter_.type_;
    
    Miro::CFG::Type const * const parameterType =
      ConfigFile::instance()->description().getType(typeName);
    
    if (parameterType == NULL) {
      throw QString("Parameter description for " + typeName +
		    " not found.\nCheck whether the relevant description file is loaded.");
    }
    
    dialog = new ParameterDialog(*parameterType,
				 tmpParentNode_, tmpNode_, 
				 NULL, NULL,
				 NULL, name());
    
  }
  else if (type_ == VECTOR) {

    // get the nested parameter type
    int len = parameter_.type_.length();
    int vlen = QString("std::vector<").length();
    len --; // tailing >
    len -= vlen;
    QString nestedType = parameter_.type_.mid(vlen, len);

    // generate nested parameter
    Miro::CFG::Parameter parameter;
    parameter.type_ = nestedType;
    parameter.name_ = parameter_.name_;
    parameter.default_ = "";
    parameter.fullDefault_ = "";
    parameter.measure_ = parameter_.measure_;
    parameter.description_ = parameter_.description_;


    dialog = new ParameterListDialog(ParameterList::VECTOR,
				     parameter,
				     tmpParentNode_, tmpNode_, 
				     NULL, NULL,
				     NULL, parameter.name_);
  }

  if (dialog) {
    int rc = dialog->exec();
    
    if (rc == QDialog::Accepted) {
      dialog->setXML();
      if (dialog->modified())
	modified_ = true;
    }
    delete dialog;
  }
}

void
DeferredParameterEdit::setXML()
{
  // no edit -> nothing to be done
  if (!modified_)
    return;

  // delete entry if edit field is empty
  if (tmpNode_.isNull() || 
      tmpNode_.firstChild().isNull()) {

    if (!node_.isNull()) {
      parentNode_.removeChild(node_);
      delete item_;
    }
    else {
      modified_ = false;
    }
    return;
  }  

  // crete a node if necessary
  if (node_.isNull()) {

    MIRO_ASSERT(!parentNode_.ownerDocument().isNull());
    QDomElement e = parentNode_.ownerDocument().createElement(XML_TAG_PARAMETER);

    e.setAttribute(XML_ATTRIBUTE_KEY, name());
    node_ = parentNode_.appendChild(e);

    MIRO_ASSERT(!node_.isNull());
  }

  //--------------------------------------
  // replace node by new content

  // remember the predecessor
  QListViewItem * pre = NULL;
  if (item_) {
    QListViewItem * parent = item_->listViewItem()->parent();
    if (parent != NULL) {
      pre = parent->firstChild();
      while (pre != NULL) {
	if (pre->nextSibling() == item_->listViewItem())
	  break;
	pre = pre->nextSibling();
      }
    }
  }

  // delete the current content
  delete item_;
  // replace the xml subtree
  QDomNode node = tmpNode_.cloneNode();
  node_.parentNode().replaceChild(node, node_);
  node_ = node;
  // reconstruct the listview if available
  if (parentItem_) {

    QString typeName = parameter_.type_;
    Miro::CFG::Type const * const parameterType =
      ConfigFile::instance()->description().getType(typeName);
    
    if (parameterType == NULL) {
      throw QString("Parameter description for " + typeName +
		    " not found.\nCheck whether the relevant description file is loaded.");
    }
    
    if (type_ == NESTED_PARAMETER) {
      item_ = new CompoundParameter(*parameterType,
				    node,
				    parentItem_->listViewItem(), pre,
				    parentItem_, name());
      dynamic_cast<CompoundParameter *>(item_)->init();
    }
    else if (type_ == VECTOR) {
    }
  }
}

bool
DeferredParameterEdit::modified()  const
{
  return modified_;
}
