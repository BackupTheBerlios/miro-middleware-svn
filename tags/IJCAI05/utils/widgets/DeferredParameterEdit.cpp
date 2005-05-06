// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DeferredParameterEdit.h"
#include "ParameterDialog.h"
#include "ParameterListDialog.h"
#include "CompoundParameter.h"
#include "ParameterList.h"

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

void printElements(QDomNode node, unsigned int i) {
  return ;

  QDomNode n = node.firstChild();
  while ( !n.isNull() ) {
    if ( n.isElement() ) {
      QDomElement e = n.toElement();
      for (unsigned int x = 0; x < i; ++x) {
	std::cout << "  ";
      }
      std::cout << "tag name: " << e.tagName();
      if (e.hasAttribute("name"))
	std::cout << " name: " << e.attribute("name");
      if (e.hasAttribute("value"))
	std::cout << " value: " << e.attribute("value");
      std::cout << std::endl;
    }
    printElements(n, i + 1);
    n = n.nextSibling();
  }
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
  QDomNode tmpNode = e;
  if (!node_.isNull()) {
    tmpNode = node_.cloneNode();
  }
  tmpParentNode_.appendChild(tmpNode);
}

void
DeferredParameterEdit::deferredEdit()
{
  DialogXML * dialog = NULL;

  QDomNode tmpNode = tmpParentNode_.firstChild();

  if (type_ == NESTED_PARAMETER) {
    QString typeName = parameter_.type_;
    
    Miro::CFG::Type const * const parameterType =
      ConfigFile::instance()->description().getType(typeName);
    
    if (parameterType == NULL) {
      throw QString("Parameter description for " + typeName +
		    " not found.\nCheck whether the relevant description file is loaded.");
    }
    
    dialog = new ParameterDialog(*parameterType,
				 tmpParentNode_, tmpNode, 
				 NULL, NULL,
				 NULL, name());
    
  }
  else if (type_ == VECTOR) {

    // get the nested parameter type
    int len = parameter_.type_.length();
    int vlen = QString("std::vector<").length();
    len --; // tailing >
    len -= vlen;
    nestedType_ = parameter_.type_.mid(vlen, len);

    // generate nested parameter
    Miro::CFG::Parameter parameter;
    parameter.type_ = nestedType_;
    parameter.name_ = parameter_.name_;
    parameter.default_ = "";
    parameter.fullDefault_ = "";
    parameter.measure_ = parameter_.measure_;
    parameter.description_ = parameter_.description_;


    dialog = new ParameterListDialog(ParameterList::VECTOR,
				     parameter,
				     tmpParentNode_, tmpNode,
				     NULL, NULL,
				     NULL, parameter.name_);
  }
  else if (type_ == SET) {

    // get the nested parameter type
    int len = parameter_.type_.length();
    int vlen = QString("std::set<").length();
    len --; // tailing >
    len -= vlen;
    nestedType_ = parameter_.type_.mid(vlen, len);

    // generate nested parameter
    Miro::CFG::Parameter parameter;
    parameter.type_ = nestedType_;
    parameter.name_ = parameter_.name_;
    parameter.default_ = "";
    parameter.fullDefault_ = "";
    parameter.measure_ = parameter_.measure_;
    parameter.description_ = parameter_.description_;


    dialog = new ParameterListDialog(ParameterList::SET,
				     parameter,
				     tmpParentNode_, tmpNode, 
				     NULL, NULL,
				     NULL, parameter.name_);
  }

  printElements(tmpParentNode_, 0);

  if (dialog) {
    int rc = dialog->exec();
    
    if (rc == QDialog::Accepted) {

      std::cout << "get parameters" << std::endl;
      dialog->setXML();
      if (dialog->modified())
	modified_ = true;
    }
    delete dialog;
  }

  printElements(tmpParentNode_, 0);
}

void
DeferredParameterEdit::setXML()
{
  QDomNode tmpNode = tmpParentNode_.firstChild();

  // no edit -> nothing to be done
  if (!modified_)
    return;

  // delete entry if edit field is empty
  if (tmpNode.isNull() || 
      tmpNode.firstChild().isNull()) {

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
    // delete the current content
    delete item_;
  }

  // replace the xml subtree
  QDomNode node = tmpNode.cloneNode();
  node_.parentNode().replaceChild(node, node_);
  node_ = node;

  // reconstruct the listview if available
  if (parentItem_) {
    item_ = NULL;
    QString typeName = parameter_.type_;
    if (type_ == NESTED_PARAMETER) {
      Miro::CFG::Type const * const parameterType =
	ConfigFile::instance()->description().getType(typeName);
      
      if (parameterType == NULL) {
	throw QString("Parameter description for " + typeName +
		      " not found.\nCheck whether the relevant description file is loaded.");
      }
    
      item_ = new CompoundParameter(*parameterType,
				    node,
				    parentItem_->listViewItem(), pre,
				    parentItem_, name());
    }
    else if (type_ == VECTOR ||
	     type_ == SET) {
      item_ = new ParameterList(parameter_, 
				node,
				parentItem_->listViewItem(), pre,
				parentItem_, name());
    }
    if (item_ != NULL)
      dynamic_cast<ParameterXML *>(item_)->init();
  }
}

bool
DeferredParameterEdit::modified()  const
{
  return modified_;
}


