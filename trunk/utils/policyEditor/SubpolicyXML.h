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
#ifndef SubpolicyXML_h
#define SubpolicyXML_h

#include "PatternXML.h"

/** This class represents all internal data of the SubpolicyEditor */
class SubpolicyXML : public PatternXML
{
  Q_OBJECT;
  
  typedef PatternXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------  

  SubpolicyXML(QDomNode& _node,
	       QListViewItem * _parentItem, QListViewItem * _pre,
	       QObject * _parent, const char * _name);

  void policyRenaming(QString const& _oldName, QString const& _newName);
  void retype(QString const& _newType);
  QString type() const;

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  

  //  virtual void contextMenu(QPopupMenu& _menu);
  //! Collect all unbound transtions from the subpolicy.
  virtual QStringVector transitionMessages() const;

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  

  static const QString XML_TAG;
  static const QString XML_ATTRIBUTE_TYPE;

protected:
  //----------------------------------------------------------------------------  
  // protected methods
  //----------------------------------------------------------------------------

  //! Build the subtree from the XML document.
  void buildSubtree();

  //----------------------------------------------------------------------------  
  // inherited protected methods
  //----------------------------------------------------------------------------

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  SubpolicyXML(SubpolicyXML const&);
};

#endif
