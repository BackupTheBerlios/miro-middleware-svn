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
#ifndef SimpleParameterEdit_h
#define SimpleParameterEdit_h

#include "ParameterEdit.h"
#include "SimpleParameter.h"

// forward declarations
class QString;
class QLineEdit;

class SimpleParameterEdit : public ParameterEdit
{
  Q_OBJECT;
  typedef ParameterEdit Super;

public:
  SimpleParameterEdit(SimpleParameter::Type _type,
		      Miro::CFG::Parameter const& _parameter,
		      QDomNode& _parentNode, QDomNode& _node,
		      ItemXML * _parentItem, ItemXML * _item,
		      QWidget * _parent, const char * _name);

  virtual void setXML();
  virtual bool modified() const;

protected:
  SimpleParameter::Type type_;

  QLineEdit * lineEdit_;
};

#endif // SimpleParameterEdit_h
