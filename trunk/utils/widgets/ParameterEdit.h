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
#ifndef ParameterEdit_h
#define ParameterEdit_h

#include <qobject.h>
#include <qdom.h>

// forward declarations
namespace Miro {
  namespace CFG {
    class Parameter;
  }
}
class ParameterDialog;
class ItemXML;

class ParameterEdit : public QObject
{
  Q_OBJECT;

  typedef QObject Super;

public:
  ParameterEdit(Miro::CFG::Parameter const& _parameter,
		QDomNode& _parentNode, QDomNode& _node,
		ItemXML * _parentItem, ItemXML * _item,
		QWidget * _parent, const char * _name);

  QWidget * editWidget();
  virtual void setXML() = 0;
  virtual bool modified() const = 0;

protected:
  Miro::CFG::Parameter const& parameter_;

  QDomNode parentNode_;
  QDomNode node_;

  QWidget * editWidget_;

  ItemXML * parentItem_;
  ItemXML * item_;

  static QString const XML_TAG_PARAMETER;
  static QString const XML_ATTRIBUTE_KEY;
  static QString const XML_ATTRIBUTE_VALUE;
};

inline
QWidget *
ParameterEdit::editWidget() {
  return editWidget_;
}
#endif // ParameterEdit_h
