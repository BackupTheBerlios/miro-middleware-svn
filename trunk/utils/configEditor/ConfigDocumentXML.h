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
#ifndef ConfigDocumentXML_h
#define ConfigDocumentXML_h

#include "utils/widgets/DocumentXML.h"

// forward declarations
class QListView;

class ConfigDocumentXML : public DocumentXML
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

  ConfigDocumentXML(QDomDocument const& _document,
		    QListView * _listView, 
		    QObject * _parent = NULL, const char * _name = NULL);
  virtual ~ConfigDocumentXML();

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

public slots:
  //----------------------------------------------------------------------------  
  // public slots
  //----------------------------------------------------------------------------  

  void onAddSection(int);

protected:
  //----------------------------------------------------------------------------  
  // protected members
  //----------------------------------------------------------------------------  

  //! The section menu.
  QPopupMenu * menuAddSection_;

  //----------------------------------------------------------------------------  
  // hidden methods
  //----------------------------------------------------------------------------  

  ConfigDocumentXML(ConfigDocumentXML const&);
};

#endif
