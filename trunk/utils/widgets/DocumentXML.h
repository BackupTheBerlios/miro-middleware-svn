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
#ifndef DocumentXML_h
#define DocumentXML_h

#include "ItemXML.h"

// forward declarations
class QListView;


class DocumentXML : public ItemXML
{
  Q_OBJECT

  typedef ItemXML Super;
public:
  DocumentXML(QDomDocument const& _document,
	      QListView * _listView, 
	      QObject * _parent = NULL, const char * _name = NULL);
  virtual ~DocumentXML();

  //! Reset policy document.
  virtual void init(QString const& _rootTag);
  //! Parse document and build list view tree
  virtual void parse() = 0;
  //! Clean up document.
  virtual void clear();
  
  //------------------//
  // document methods //
  //------------------//

  //! Set the name of the document.
  void setName(const QString& _name);
  //! Load a new document from a given XML file.
  void loadXML(const QString& _name);
  //! Save the document to a given XML file.
  void saveXML();

  //! Accessor method to the xml document.
  QDomDocument const& domDocument() const;

protected:
  //! The main document structure.
  QDomDocument document_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  DocumentXML(DocumentXML const&);
};

inline
QDomDocument const& 
DocumentXML::domDocument() const {
  return document_;
}
#endif
