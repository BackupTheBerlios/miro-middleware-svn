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
#ifndef ItemXML_h
#define ItemXML_h

#include "Item.h"

#include <qstring.h>
#include <qdom.h>

//! This class represents an item in the XML tree.
/**
 * It encapsulates the xml processing of the item.
 */
class ItemXML : public Item
{
  Q_OBJECT;

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef Item Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  /**
   * @param node Reference to the node in the xml document
   * representing the item.
   */
  ItemXML(QDomNode const& _node,
	  QListViewItem * _parentItem, QListViewItem * _pre = NULL,
	  QObject * _parent = NULL, const char * _name = NULL);
  //! Initializing constructor.
  /**
   * @param node Reference to the node in the xml document
   * representing the item.
   */
  ItemXML(QDomNode const& _node,
	  QListView * _view, QListViewItem * _pre = NULL,
	  QObject * _parent = NULL, const char * _name = NULL);
  //! Virtual destructor.
  virtual ~ItemXML();


  //! Rename item within the document.
  virtual void rename(const QString& _name);
  //! Move item within the document.
  virtual void moveTo(QDomNode& _parent, QDomNode& _predecessor);
  //! Test if item is deletable.
  virtual bool deletable();
  //! Delete item from the document.
  virtual void deleteItem();

  //! The node associated with the item.
  QDomNode node() const;

  //! Report if the node or one of its childs was modified.
  bool modified() const;

  //! Associate a widget with an ItemXML.
  /** This is usually used to edit the underlying QDomNode. */
  void setWidget(QWidget * _widget);
  //! Accessor for the associated widget.
  /** Note that the return value can be NULL. */
  QWidget * widget();

  //----------------------------------------------------------------------------
  // inherited public methods
  //----------------------------------------------------------------------------

  //! Provide the context menu.
  virtual void contextMenu(QPopupMenu& _menu);
  //! Move item up in the list view and in the document.
  virtual void moveUp();
  //! Move item down  in the list view and in the document.
  virtual void moveDown();

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------

  //! The xml attribute for key values.
  static QString const XML_ATTRIBUTE_KEY;

public slots:
  //! Delete element from the document.
  void slotDelete();

protected slots:
  //! Set the name attribute of the element.
  void slotRename();

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------

  //! Set the modified bit
  void setModified(bool _modified = true, bool _recurse = false);

  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! Reference to the node in the xml document representing the item.
  QDomNode node_;
  //! an associated widget that needs to be updated due to changes here
  QWidget * widget_;

private:
  //----------------------------------------------------------------------------
  // private data
  //----------------------------------------------------------------------------

  //! Modification indicator.
  bool modified_;

  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  ItemXML(ItemXML const&);
};

inline
QDomNode
ItemXML::node() const {
  return node_;
}
inline
void
ItemXML::setWidget(QWidget * _widget) {
  widget_ = _widget;
}
inline
QWidget *
ItemXML::widget() {
  return widget_;
}
#endif
