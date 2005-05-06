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
#ifndef ExternalTransitionXML_h
#define ExternalTransitionXML_h

#include "utils/widgets/ItemXML.h"

// forward declarations
class PolicyDocumentXML;

class ExternalTransitionXML : public ItemXML
{
  Q_OBJECT;
  
  typedef ItemXML Super;

public:
  //----------------------------------------------------------------------------  
  // public methods
  //----------------------------------------------------------------------------

  ExternalTransitionXML(QDomNode& _node,
			QListViewItem * _parentItem, QListViewItem * _pre,
			QObject * _parent, const char * _name);

  PolicyDocumentXML * document() const;

  QString message();

  //----------------------------------------------------------------------------  
  // inherited public methods
  //----------------------------------------------------------------------------

  void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------  
  // public constants
  //----------------------------------------------------------------------------  
  static const QString XML_TAG;
  static const QString XML_ATTRIBUTE_MSG;

public slots:
  void setMessage();

private:
  //----------------------------------------------------------------------------  
  // hidden members
  //----------------------------------------------------------------------------
  ExternalTransitionXML(ExternalTransitionXML const&);
};

#endif
