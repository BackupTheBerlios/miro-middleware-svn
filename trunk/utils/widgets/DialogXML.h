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
#ifndef DialogXML_h
#define DialogXML_h

#include <qdialog.h>
#include <qdom.h>

#include <iostream>

// forward declarations
class QFrame;
class QGroupBox;
class ItemXML;

//! Dialog base class, for editing XML nodes of the parameter framework.
class DialogXML : public QDialog
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef QDialog Super;

public:

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  DialogXML(QDomNode const& _parentNode, QDomNode const& _node,
	    ItemXML * _parentItem, ItemXML * _item,
	    QWidget * _parent, const char * _name, bool _modal = true);

  virtual void setXML() = 0;

  bool modified() const;
  
signals:
  //----------------------------------------------------------------------------
  // signals
  //----------------------------------------------------------------------------

  //! Emitted, if the input of the dialog is acceptable.
  void okay(bool);

protected slots:
  //----------------------------------------------------------------------------
  // protected slots
  //----------------------------------------------------------------------------

  //! Inherited method.
  /** Adds no functionality. Duplicated only, to avoid warning about
   *  hidden virtual.
   */ 
  virtual void accept();
  //! Tell the dialog to accept the input or not.
  /**
   * This is used for a voting method on all child edit fields.
   * The input is only accepted if noone objects.
   */
  void accept(bool _accept);

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------

  //! Set modified flag.
  void setModified(bool _modified);

  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! Parent node in the XML tree.
  /** Needed if there is no child node for the parameter itself yet. */
  QDomNode parentNode_;
  //! XML node of the parameter.
  QDomNode node_;

  //! Parent Item in the QListView.
  /** Needed if there is no item for the parameter itself yet. */
  ItemXML * parentItem_;
  //! Parameter item.
  ItemXML * item_;

  //! Layout element needed.
  QGroupBox * groupBox_;
  //! Layout element.
  QFrame * frame_;

private:
  //----------------------------------------------------------------------------
  // private data
  //----------------------------------------------------------------------------

  //! Flag indicating the dialog was modified.
  bool modified_;
  //! Acceptance counter.
  /** accept(false) increments, accept(true) decrements.
   * accept_ == 0 => okay button active
   */
  int accept_;
};

inline
bool
DialogXML::modified() const {
  return modified_;
}
inline
void
DialogXML::setModified(bool _modified) {
  modified_ = _modified;
}
#endif
