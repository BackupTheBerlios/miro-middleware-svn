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


//! Class representing an XML document.
/** 
 * It has also an associated QListViewItem, that represents the node
 * in a QListView.
 */
class DocumentXML : public ItemXML
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ItemXML Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor. Taking a XML document as first parameter.
  DocumentXML(QDomDocument const& _document,
	      QListView * _listView, 
	      QObject * _parent = NULL, const char * _name = NULL);
  //! Virtual destructor.
  virtual ~DocumentXML();

  //! Reset policy document.
  virtual void init(QString const& _rootTag);
  //! Set policy document.
  virtual void initXML(QString const& _xml);
  //! Parse document and build list view tree
  virtual void parse() = 0;
  //! Clean up document.
  virtual void clear();
  
  //! Set the name of the document.
  void setName(const QString& _name);
  //! Load a new document from a given XML file.
  void loadXML(const QString& _name);
  //! Save the document to a given XML file.
  void saveXML();

  //! Accessor method to the xml document.
  QDomDocument const& domDocument() const;

protected:
  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

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
