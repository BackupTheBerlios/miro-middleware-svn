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

#include "ParameterInstance.h"

QString const ParameterInstance::XML_TAG = "instance";
QString const ParameterInstance::XML_ATTRIBUTE_TYPE = "type";

ParameterInstance::ParameterInstance(QListViewItem * _listViewItem, 
				     QListViewItem * _pre,
				     QDomNode const& _node,
				     QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_TYPE),
	_node,
	_listViewItem, _pre, _parent, _name)
{
}

