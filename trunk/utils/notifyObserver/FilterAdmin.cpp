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

#include "FilterAdmin.h"

FilterAdmin::FilterAdmin(CosNotifyFilter::FilterAdmin_ptr _filterAdmin,
			 QObject * _parent, const char * _name) :
  Super(_parent, _name),
  filterAdmin_(CosNotifyFilter::FilterAdmin::_duplicate(_filterAdmin))
{}
