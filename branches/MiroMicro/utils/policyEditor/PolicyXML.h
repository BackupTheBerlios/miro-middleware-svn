// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef PolicyXML_h
#define PolicyXML_h

#include "PatternXML.h"

// forward declarations
class PolicyDocumentXML;

class PolicyXML : public PatternXML
{
  Q_OBJECT;
  
  //----------------------------------------------------------------------------  
  // private types
  //----------------------------------------------------------------------------

  typedef PatternXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------

  PolicyXML(QDomNode const& _node,
	    QListViewItem * _parentItem, QListViewItem * _pre,
	    QObject * _parent, const char * _name);
  ~PolicyXML();

  PolicyDocumentXML * document();

  QStringList patternList();
  bool uniquePatternName(QString const& _name);
  void policyRenaming(QString const& _oldName, QString const& _newName);
  void removeTransitionTo(QString const& _target);
  void renameTransitionTo(QString const& _oldTarget, QString const& _newTarget);
  PatternXML * patternFromName(QString const& _name);

  void addDownTransition(QString const& _msg, QString const& _target);
  bool tryAddDownTransition(QString const& _msg, PatternXML * _target);

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  

  virtual void contextMenu(QPopupMenu& _menu);
  virtual void rename(QString const& _name);
  virtual void moveUp();
  virtual void moveDown();
  virtual bool deletable();
  virtual void deleteItem();
  //! Collect all unbound transtions from the subpolicy.
  virtual QStringVector transitionMessages() const;
  virtual QStringVector boundTransitionMessages() const;

  QStringVector unboundDownTransitionMessages() const;

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;

protected slots:
  //----------------------------------------------------------------------------  
  // protected slots
  //----------------------------------------------------------------------------
  void addActionPattern();
  void addSubpolicy(int _id);
  void addDownTransition();

protected:
  //----------------------------------------------------------------------------  
  // protected methods
  //----------------------------------------------------------------------------

  //! Build the subtree from the XML document.
  void buildSubtree();
  //! Test if subpolicy _name is used within the policy.
  bool hasSubpolicy(QString const& _name) const;
  //! Test if a pattern _name exists in the policy.
  bool hasPattern(QString const& _name) const;


  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------

  QPopupMenu * menuAddSubpolicy_;
  QPopupMenu * menuAddDownTransition_;

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  PolicyXML(PolicyXML const&);
};

#endif
