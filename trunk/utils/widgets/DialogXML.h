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
#ifndef DialogXML_h
#define DialogXML_h

#include <qdialog.h>
#include <qdom.h>

#include <iostream>

// forward declarations
class QFrame;
class QGroupBox;
class ItemXML;

class DialogXML : public QDialog
{
  Q_OBJECT

  typedef QDialog Super;

public:
  DialogXML(QDomNode const& _parentNode, QDomNode const& _node,
	    ItemXML * _parentItem, ItemXML * _item,
	    QWidget * _parent, const char * _name, bool _modal = true);

  virtual void setXML() = 0;

  bool modified() const;
  
protected slots:
  void accept(bool _accept);

signals:
  void okay(bool);

protected:
  void setModified(bool _modified);

  QDomNode parentNode_;
  QDomNode node_;

  ItemXML * parentItem_;
  ItemXML * item_;

  QGroupBox * groupBox_;
  QFrame * frame_;

private:
  bool modified_;
  int accept_;
};

inline
bool
DialogXML::modified() const {
  return modified_;
}
inline
void
DialogXML::setModified(bool _modified) {
  modified_ = _modified;
}
#endif
