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

#include <string>
#include <vector>

// forward declarations
class QString;
class QLineEdit;
class QComboBox;
class QListBox;
class ConfigFile;


class SimpleParameterEdit : public ParameterEdit
{
  Q_OBJECT;

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef ParameterEdit Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  SimpleParameterEdit(SimpleParameter::Type _type,
		      Miro::CFG::Parameter const& _parameter,
		      QDomNode& _parentNode, QDomNode& _node,
		      ItemXML * _parentItem, ItemXML * _item,
		      QWidget * _parent, const char * _name);

  //! Inherited interface.
  virtual void setXML();
  //! Inherited interface.
  virtual bool modified() const;

protected slots:
  void typeBoxModified();
  void listBoxModified();

protected:

  std::vector<std::string> fullDef2StringVector(std::string const& _string);
  std::vector<std::string> tokenizer(std::string _values);

  //----------------------------------------------------------------------------
  // puprotected members
  //----------------------------------------------------------------------------

  ConfigFile * config_;

  SimpleParameter::Type type_;

  QLineEdit * lineEdit_;
  QComboBox * typeBox_;
  QListBox * listBox_;
  bool typeBoxModified_;
  bool listBoxModified_;
};

#endif // SimpleParameterEdit_h
