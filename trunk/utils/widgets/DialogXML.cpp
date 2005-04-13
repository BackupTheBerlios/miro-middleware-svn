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

#include "DialogXML.h"
#include "ItemXML.h"

#include "miro/Log.h"

DialogXML::DialogXML(QDomNode const& _parentNode, QDomNode const& _node,
		     ItemXML * _parentItem, ItemXML * _item,
		     QWidget * _parent, const char * _name, bool _modal) :
  Super(_parent, _name, _modal),
  parentNode_(_parentNode),
  node_(_node),
  parentItem_(_parentItem),
  item_(_item)
{
}
