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
#ifndef DocumentView_h
#define DocumentView_h

#include "DocumentXML.h"

#include <qlistview.h>

// forward declarattions
class DocumentFile;
class QWidget;
class QStatusBar;

//! Main class of the DocumentView application 
class DocumentView : public QListView
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! The super class.
  typedef QListView Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  DocumentView(QWidget * _parent = 0, char const * _name = 0, WFlags _f = 0);
  virtual ~DocumentView();

  void newDocument();
  void openDocument(QString const& _name);
  bool saveDocument();
  bool saveDocumentAs();

  void setDocument(DocumentXML * _document, QString const& _rootTag);
  void setTitleBar(QWidget * _titleBar);
  void setStatusBar(QStatusBar * _statusBar);
  void tryClose(QCloseEvent *e);

protected slots:

  // file menu
  void slotNew();
  void slotLoad();
  void slotSave();
  void slotSaveAs();

  // interaction
  void slotContextMenu(QListViewItem * _item, const QPoint & _pos, int);
  void slotDoubleClick(QListViewItem * _item);

protected:
  //----------------------------------------------------------------------------
  // protected methods data
  //----------------------------------------------------------------------------
  bool saveIfModified();
  void setMessage(QString const& _message, int _ms);
  void setTitle(QString const& _title);

  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! The main window.
  QWidget * titleBar_;
  //! The status bar.
  QStatusBar * statusBar_;

  //! Document file.
  DocumentXML * document_;
  //! The root tag of the document.
  QString rootTag_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  DocumentView(DocumentView const&);
};

inline
void
DocumentView::setTitleBar(QWidget * _titleBar) {
  titleBar_ = _titleBar;
}

inline
void
DocumentView::setStatusBar(QStatusBar * _statusBar) {
  statusBar_ = _statusBar;
}


#endif

