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
#ifndef ParameterXML_h
#define ParameterXML_h

#include "ItemXML.h"

#include "params/Type.h"

#include <qstring.h>

// forward declarations
class ConfigFile;
class QListView;
class QListViewItem;

class ParameterXML : public ItemXML
{
  Q_OBJECT
  
  typedef ItemXML Super;
public:

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  ParameterXML(QDomNode const& _node,
	       QListViewItem * _parentItem, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  ParameterXML(QDomNode const& _node,
	       QListView * _view, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  virtual void init();
  virtual void contextMenu(QPopupMenu& _menu);
  virtual void setParameters();

  //----------------------------------------------------------------------------
  // public constants
  //----------------------------------------------------------------------------

  static QString const XML_TAG;

public slots:
  void slotSetParameters();

protected:

  //----------------------------------------------------------------------------
  // puprotected members
  //----------------------------------------------------------------------------

  ConfigFile * config_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  ParameterXML(ParameterXML const&);
};

#endif // ParameterXML_h
