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
#ifndef PolicyDocumentXML_h
#define PolicyDocumentXML_h

#include "utils/widgets/DocumentXML.h"

#include <qstringlist.h>

//#include <vector>

// forward declarations
class QListView;
class PolicyXML;

//typedef std::vector<QString> QStringVector;

/** This class represents all internal data of the PolicyEditor */
class PolicyDocumentXML : public DocumentXML
{
  Q_OBJECT

  //----------------------------------------------------------------------------  
  // private types
  //----------------------------------------------------------------------------  
  
  typedef DocumentXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------  
  PolicyDocumentXML(QDomDocument const& _document,
		    QListView * _listView, 
		    QObject * _parent = NULL, const char * _name = NULL);
  ~PolicyDocumentXML();

  bool uniqueName(const QString & _policyName) const;
  bool structuredPolicy() const;
  void policyRenaming(QString const& _oldName, QString const& _newName);
  QStringVector policies(QString const& _end) const;
  QStringVector externalMessages() const;
  bool hasExternalMessage(QString const& _msg) const;
  PolicyXML * policyByName(QString const & _name);



  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------  

  virtual void parse();
  virtual void contextMenu(QPopupMenu& _menu);
  
  //----------------------------------------------------------------------------  
  // public constants
  //---------------------------------------------------------------------------- 
 
  static const QString XML_DOCTYPE;
  static const QString XML_TAG;
  static const QString XML_TAG_FLATPOLICY;

public slots:
  //----------------------------------------------------------------------------  
  // public slots
  //----------------------------------------------------------------------------  

  void slotAddPolicy();
  void slotAddExternalMessage();

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  PolicyDocumentXML(PolicyDocumentXML const&);
};

#endif // PolicyDocumentXML
