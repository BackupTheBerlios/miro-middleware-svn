// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DownTransitionXML_h
#define DownTransitionXML_h

#include "TransitionXML.h"

// forward declarations
class PolicyXML;

class DownTransitionXML : public TransitionXML
{
  Q_OBJECT;
  
  typedef TransitionXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------

  DownTransitionXML(QDomNode& _node,
		QListViewItem * _parentItem, QListViewItem * _pre,
		QObject * _parent, const char * _name);

  PolicyXML * policy() const;

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------

  void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;

protected:
  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------
  virtual void updateView();

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  DownTransitionXML(DownTransitionXML const&);
};

#endif
