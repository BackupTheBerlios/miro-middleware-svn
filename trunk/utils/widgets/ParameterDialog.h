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
#ifndef ParameterDialog_h
#define ParameterDialog_h

#include "DialogXML.h"
#include "params/Type.h"

#include <vector>

// forward declarations
class ParameterEdit;
class ConfigFile;

/**
 * This class shows a dialog for the behaviour parameters.
 */
class ParameterDialog : public DialogXML
{
  Q_OBJECT

  typedef DialogXML Super;
  typedef std::vector<ParameterEdit *> EditVector;

public:
  ParameterDialog(Miro::CFG::Type const& _parameterType,
		  QDomNode const& _parentNode, QDomNode const& _node,
		  ItemXML * _parentItem, ItemXML * _item,
		  QWidget * _parent, const char * _name);

  virtual void setXML();

protected:
  void initDialog();

  ConfigFile * config_;

  Miro::CFG::Type parameterType_;
  Miro::CFG::Type::ParameterSet params_;
  
private:
  EditVector editFelds_;
};

#endif
