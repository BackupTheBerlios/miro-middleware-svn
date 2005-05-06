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
