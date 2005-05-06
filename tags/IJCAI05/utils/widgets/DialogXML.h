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

#include "OkCancelDialog.h"

#include <qdom.h>

#include <iostream>

class ItemXML;

//! Dialog base class, for editing XML nodes of the parameter framework.
class DialogXML : public OkCancelDialog
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef OkCancelDialog Super;

public:

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  DialogXML(QDomNode const& _parentNode, QDomNode const& _node,
	    ItemXML * _parentItem, ItemXML * _item,
	    QWidget * _parent, const char * _name, bool _modal = true);

  virtual void setXML() = 0;

protected:
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
};

#endif
