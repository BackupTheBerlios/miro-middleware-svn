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

#include "ParameterSingleton.h"

ParameterSingleton::ParameterSingleton(QListViewItem * _listViewItem, 
				       QListViewItem * _pre,
				       QDomNode const& _node,
				       QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_KEY),
	_node, 
	_listViewItem, _pre, _parent, _name)
{
}
