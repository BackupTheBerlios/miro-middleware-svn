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
#ifndef Section_h
#define Section_h

#include "utils/widgets/ItemXML.h"

#include <qstring.h>

// forward declarations
class QListViewItem;

class Section : public ItemXML
{
  Q_OBJECT
  
  typedef ItemXML Super;
public:
  Section(QDomNode const& _node,
	  QListViewItem * _parentItem, QListViewItem * _pre,
	  QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  
  virtual void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------

  static QString const XML_TAG;

protected slots:
  //! Add a named instance of a parameter type.
  void onAddInstance(int _n);
  //! Add a default instance of a parameter.
  void onAddParameter(int _n);

protected:
  //! Build the subtree from the XML document.
  void buildSubtree();

  QPopupMenu * menuAddParameter_;
  QPopupMenu * menuAddInstance_;
};

#endif
