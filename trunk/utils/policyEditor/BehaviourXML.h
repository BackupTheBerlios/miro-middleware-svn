// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BehaviourXML_h
#define BehaviourXML_h

#include "utils/widgets/CompoundParameter.h"

#include <qstringlist.h>

// forward declarations
class PatternWidget;
class BehaviourWidget;

class BehaviourXML : public CompoundParameter
{
  Q_OBJECT;
  //----------------------------------------------------------------------------  
  // private types
  //----------------------------------------------------------------------------  

  typedef CompoundParameter Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------  

  BehaviourXML(QDomNode& _node,
	       QListViewItem * _parentItem, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  QStringVector transitionMessages() const;

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  

  void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;
};

#endif // Behaviour_XML
