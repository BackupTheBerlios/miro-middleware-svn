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

#include "ParameterEdit.h"

#include "params/Parameter.h"

#include "miro/Log.h"

#include <qwidget.h>

QString const ParameterEdit::XML_TAG_PARAMETER = "parameter";
QString const ParameterEdit::XML_ATTRIBUTE_KEY = "name";
QString const ParameterEdit::XML_ATTRIBUTE_VALUE = "value";


ParameterEdit::ParameterEdit(Miro::CFG::Parameter const& _parameter,
			     QDomNode& _parentNode, QDomNode& _node,
			     ItemXML * _parentItem, ItemXML * _item,
			     QWidget * _parent, const char * _name) :
  Super(_parent, _name),
  parameter_(_parameter),
  parentNode_(_parentNode),
  node_(_node),
  parentItem_(_parentItem),
  item_(_item)
{
  MIRO_ASSERT(!parentNode_.isNull());
}
