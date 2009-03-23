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
