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
#ifndef ParameterInstance_h
#define ParameterInstance_h

#include "utils/widgets/CompoundParameter.h"

// forward declarations
class ConfigFile;
class QListViewItem;

class ParameterInstance : public CompoundParameter
{
  Q_OBJECT
  
  typedef CompoundParameter Super;
public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------
  ParameterInstance(QListViewItem * _listViewItem, QListViewItem * _pre,
		    QDomNode const& _node,
		    QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  //! Inherited method.
  virtual void moveUp();
  //! Inherited method.
  virtual void moveDown();
  //! Inherited method.
  virtual void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------
  static QString const XML_TAG;
  static QString const XML_ATTRIBUTE_TYPE;
};

#endif // ParameterInstance_h
