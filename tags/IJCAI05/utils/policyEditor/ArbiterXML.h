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
#ifndef ArbiterXML_h
#define ArbiterXML_h

#include "utils/widgets/CompoundParameter.h"

/** This class represents all internal data of the ArbiterEditor */
class ArbiterXML : public CompoundParameter
{
  Q_OBJECT;
  
  typedef CompoundParameter Super;

public:
  ArbiterXML(QDomNode& _node,
	     QListViewItem * _parentItem, QListViewItem * _pre,
	     QObject * _parent, const char * _name);

  //----------------------------------------------------------------------------
  // public inherited methods
  //----------------------------------------------------------------------------

  virtual void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;

};

#endif // ArbiterXML_h
