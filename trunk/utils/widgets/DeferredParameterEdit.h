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
#ifndef DeferredParameterEdit_h
#define DeferredParameterEdit_h

#include "ParameterEdit.h"

// forward declarations
class ConfigFile;
class QPushButton;
class QString;

class DeferredParameterEdit : public ParameterEdit
{
  Q_OBJECT;
  typedef ParameterEdit Super;

public:
  enum EditType {
    NESTED_PARAMETER, VECTOR, SET
  };

  DeferredParameterEdit(EditType _type,
			Miro::CFG::Parameter const& _parameter,
			QDomNode& _parentNode, QDomNode& _node,
			ItemXML * _parentItem, ItemXML * _item,
			QWidget * _parent, const char * _name);

  virtual void setXML();
  virtual bool modified() const;

  static EditType editType(QString const& _name);

protected slots:
  void deferredEdit();

protected:
  EditType type_;
  bool modified_;
  ConfigFile * config_;

  QDomDocument tmpDocument_;
  QDomNode tmpParentNode_;
  QDomNode tmpNode_;

  QPushButton * button_;
};

#endif // DeferredParameterEdit_h
