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
#ifndef ActionPatternXML_h
#define ActionPatternXML_h

#include "PatternXML.h"

class ActionPatternXML : public PatternXML
{
  Q_OBJECT;
  
  typedef PatternXML Super;
public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------  
  ActionPatternXML(QDomNode const& _node,
		   QListViewItem * _parentItem, QListViewItem * _pre,
		   QObject * _parent, const char * _name);

  bool hasBehaviour(QString const& _name) const;

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------

  virtual void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;

protected slots:
  void setArbiter(int _id);
  void addBehaviour(int _id);

protected:
  //----------------------------------------------------------------------------  
  // protected methods
  //----------------------------------------------------------------------------

  //! Build the subtree from the XML document.
  void buildSubtree();

  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------
  QPopupMenu * menuSetArbiter_;
  QPopupMenu * menuAddBehaviour_;

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  ActionPatternXML(ActionPatternXML const&);
};

#endif
