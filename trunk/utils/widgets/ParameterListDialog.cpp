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

#include "ParameterListDialog.h"
#include "SimpleParameter.h"
#include "CompoundParameter.h"
#include "SingleParameterDialog.h"
#include "ParameterDialog.h"
#include "ConfigFile.h"

#include "miro/Log.h"
#include "params/Generator.h"

#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlistview.h>
#include <qpopupmenu.h>

ParameterListDialog::ParameterListDialog(ParameterList::Type _type,
					 Miro::CFG::Parameter const& _parameter,
					 QDomNode const& _parentNode,
					 QDomNode const& _node,
					 ItemXML * _parentItem,
					 ItemXML * _item,
					 QWidget * _parent, const char * _name) :
  Super(_parentNode, _node, 
	_parentItem, _item,
	_parent, _name, TRUE),       // TRUE = modal dialog
  type_(_type),
  parameter_(_parameter),
  nestedCompound_(true),
  nestedType_(NULL),
  index_(0),
  list_(NULL),
  delButton_(NULL),
  editButton_(NULL),
  collector_()
{

  //----------------------------------------------------------------------------
  // create a copy of the parameters xml tree
  tmpParentNode_ = tmpDocument_.createElement("section");
  tmpDocument_.appendChild(tmpParentNode_);

  QDomElement e = tmpDocument_.createElement(ParameterXML::XML_TAG);
  e.setAttribute(ParameterXML::XML_ATTRIBUTE_KEY, name());
  tmpNode_ = e;
  if (!node_.isNull())
    tmpNode_ = node_.cloneNode();
  tmpParentNode_.appendChild(tmpNode_);


  //----------------------------------------------------------------------------
  // complete the dialog widgets

  // this is a widget of the parent constructor that we do not use
  delete frame_;

  list_ = new QListView(groupBox_, "list");
  list_->addColumn("Position");
  list_->addColumn(parameter_.type_);
  list_->setSorting(-1);
  list_->setResizeMode(QListView::AllColumns);
  list_->setRootIsDecorated(true);

  QHBox * fileButtonsBox = new QHBox(groupBox_, "fileButtons");
  QPushButton * addButton = new QPushButton("Add...", fileButtonsBox);
  delButton_ = new QPushButton("Delete", fileButtonsBox);
  editButton_ = new QPushButton("Edit", fileButtonsBox);


  //----------------------------------------------------------------------------
  // add the list view items

  // get nested parameter type cathegory
  if (SimpleParameter::typeFromName(parameter_.type_) !=
      SimpleParameter::NONE) {
    nestedCompound_ = false;
  }
  else {
    nestedType_ = 
      ConfigFile::instance()->description().getType(parameter_.type_);
    if (nestedType_ == NULL) {
      throw QString("Parameter description for " + 
		    parameter_.type_ +
		    " not found.\nCheck whether the relevant description file is loaded.");
    }
  }
  
  // iterate over all members
  QDomNode n = tmpNode_.firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	e.tagName() == ParameterXML::XML_TAG) {
      
      ParameterXML * newParam = NULL;

      QString indexName;
      indexName.setNum(index_);

      if (nestedCompound_ == false) {

	if (!e.hasAttribute(SimpleParameter::XML_ATTRIBUTE_VALUE))
	  throw QString("Parameter tag without value in (" + 
			parameter_.type_ + ") " + name());

	
	QString value = e.attribute(SimpleParameter::XML_ATTRIBUTE_VALUE);
	newParam = new SimpleParameter(parameter_,
				       n,
				       list_, pre,
				       &collector_, indexName);
      }
      else {
	newParam = new CompoundParameter(*nestedType_,
					 n,
					 list_, pre,
					 &collector_, indexName);
      }

      newParam->init();
      pre = newParam->listViewItem();
      ++index_;
    }
    n = n.nextSibling();
  }


  //----------------------------------------------------------------------------
  // connect the dialogs functionality

  // enable/disable buttons
  selectListItem();

  connect(list_, 
	  SIGNAL(contextMenuRequested(QListViewItem *, const QPoint&, int)),
	  this,
	  SLOT(contextMenu(QListViewItem *, const QPoint&, int)));
  connect(list_, SIGNAL(doubleClicked(QListViewItem *)),
	  this, SLOT(slotDoubleClick(QListViewItem *)));
  connect(addButton, SIGNAL(clicked()), SLOT(add()));
  connect(delButton_, SIGNAL(clicked()), SLOT(del()));
  connect(editButton_, SIGNAL(clicked()), SLOT(edit()));

  groupBox_->sizeHint(); // we need this to get the right size (hutz)
  groupBox_->setTitle((type_ == ParameterList::SET)? "Set" : "Vector");
}

void
ParameterListDialog::setXML()
{
  // no edit -> nothing to be done
  if (!modified())
    return;

  // delete entry if edit field is empty
  if (tmpNode_.isNull() || 
      tmpNode_.firstChild().isNull()) {

    if (!node_.isNull()) {
      parentNode_.removeChild(node_);
      delete item_;
    }
    else {
      setModified(false);
    }
    return;
  }  

  // crete a node if necessary
  if (node_.isNull()) {

    MIRO_ASSERT(!parentNode_.ownerDocument().isNull());
    QDomElement e = parentNode_.ownerDocument().createElement(ParameterXML::XML_TAG);

    e.setAttribute(ParameterXML::XML_ATTRIBUTE_KEY, name());
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
  QDomNode oldNode = node_.parentNode().replaceChild(node, node_);

  MIRO_ASSERT(!oldNode.isNull());

  // reconstruct the listview if available
  if (parentItem_) {
    // generate original parameter
    Miro::CFG::Parameter parameter;

    MIRO_ASSERT(type_ != ParameterList::NONE);
    if (type_ == ParameterList::VECTOR) {
      parameter.type_ = "std::vector<" + parameter_.type_ + ">";
    }
    else if (type_ == ParameterList::SET) {
      parameter.type_ = "std::set<" + parameter_.type_ + ">";
    }

    parameter.name_ = parameter_.name_;
    parameter.default_ = "";
    parameter.fullDefault_ = "";
    parameter.measure_ = parameter_.measure_;
    parameter.description_ = parameter_.description_;

    QString p = node.toElement().attribute(ParameterXML::XML_ATTRIBUTE_KEY);
    item_ = new ParameterList(parameter,
			      node,
			      parentItem_->listViewItem(), pre,
			      parentItem_, p);
    dynamic_cast<ParameterXML *>(item_)->init();
  }
}

void
ParameterListDialog::selectListItem()
{
  if (list_->childCount() != 0) {
    QListViewItem * item = list_->currentItem();
    list_->setSelected(item, true);
  }

  delButton_->setEnabled(list_->currentItem() != NULL);
  editButton_->setEnabled(list_->currentItem() != NULL);
}

void 
ParameterListDialog::add()
{
  ParameterXML * newParam = NULL;
  QListViewItem * pre = list_->selectedItem();
  ItemXML * preItem = NULL;

  if (pre != NULL) {
    while (pre->parent()) {
      pre = pre->parent();
    }

    Item::ItemMap::const_iterator i = Item::itemMap().find(pre);
    if (i != Item::itemMap().end())
      preItem = dynamic_cast<ItemXML * >(i->second);
  }

  //----------------------------------------------------------------------------
  // insert element into document
  // it will be deleted afterwards, if nothing was enteres.
  QDomElement e = tmpDocument_.createElement(ParameterXML::XML_TAG);

  // determine where to put it
  QDomNode n = tmpNode_.firstChild();
  if (preItem != NULL) { // is no direct child of item_->node()
    n = preItem->node();
    MIRO_ASSERT(!n.isNull());
  }

  QDomNode newChild = tmpNode_.insertAfter(e, n);

  MIRO_ASSERT(!e.isNull());
  MIRO_ASSERT(!newChild.isNull());

  //----------------------------------------------------------------------------
  // set parameter value

  DialogXML * dialog = NULL;
  if (!nestedCompound_) {
    dialog = new SingleParameterDialog(parameter_,
				       newChild,
				       item_, NULL, NULL, name());
  }
  else {
    dialog = new ParameterDialog(*nestedType_,
				 newChild.parentNode(), newChild, 
				 item_, NULL, NULL, name());
  }
  int rc = dialog->exec();
  if (rc == QDialog::Accepted) {
    dialog->setXML();
    if (dialog->modified())
      setModified(true);
  }
  delete dialog;

  e = newChild.toElement();
  MIRO_ASSERT(!e.isNull());

  // if nothing is entered, remove the node
  // (that is: no value tag and no childs)
  if (!e.hasAttribute(SimpleParameter::XML_ATTRIBUTE_VALUE) &&
      e.firstChild().isNull()) {
    tmpNode_.removeChild(newChild);
    return;
  }


  //----------------------------------------------------------------------------
  //create listview item

  QString indexName;
  indexName.setNum(index_);

  if (!nestedCompound_) {
    QString value = e.attribute(SimpleParameter::XML_ATTRIBUTE_VALUE);
    newParam = new SimpleParameter(parameter_,
				   newChild, 
				   list_, pre,
				   &collector_, indexName);
  }
  else {
    newParam = new CompoundParameter(*nestedType_, 
				     newChild,
				     list_, pre,
				     &collector_, indexName);
  }

  newParam->init();
  ++index_;

  setModified(true);
  list_->setSelected(newParam->listViewItem(), true);
  selectListItem();
  renumberEntries();
}

void 
ParameterListDialog::del()
{
  QListViewItem * item; 
  if ((item = list_->currentItem()) != NULL) {
    Item::ItemMap::const_iterator i = Item::itemMap().find(item);
    MIRO_ASSERT(i != Item::itemMap().end());


    ItemXML * ix = dynamic_cast<ItemXML * > (i->second);
    ix->deleteItem();
    setModified(true);
  }
 
  selectListItem();
}

void
ParameterListDialog::edit()
{
  QListViewItem * item = list_->currentItem();
  if (item != NULL) {
    Item::ItemMap::const_iterator i = Item::itemMap().find(item);
    MIRO_ASSERT(i != Item::itemMap().end());

    ParameterXML * p = dynamic_cast<ParameterXML *>(i->second);
    MIRO_ASSERT(p != NULL);

    p->setParameters();

    if (p->modified())
      setModified(true);
  }
}

void
ParameterListDialog::up()
{}

void
ParameterListDialog::down()
{}


void
ParameterListDialog::slotDoubleClick(QListViewItem * _item)
{
  Item::ItemMap::const_iterator i = Item::itemMap().find(_item);
  MIRO_ASSERT(i != Item::itemMap().end());

  ParameterXML * p = dynamic_cast<ParameterXML *>(i->second);
  MIRO_ASSERT(p != NULL);

  p->setParameters();
}

void
ParameterListDialog::contextMenu(QListViewItem * _item, const QPoint& pos, int)
{
  Item::ItemMap::const_iterator i = Item::itemMap().find(_item);
  MIRO_ASSERT(i != Item::itemMap().end());

  QPopupMenu menu(NULL, "plistpopu");
  i->second->contextMenu(menu);

  menu.exec(pos);
}

void
ParameterListDialog::renumberEntries() 
{
  int counter = 0;
  QString number;
  QListViewItem * item = list_->firstChild();
  while (item != NULL) {
    number.setNum(counter);
    item->setText(0, number);

    item = item->nextSibling();
    ++counter;
  }
}
