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
#ifndef FilterAdmin_h
#define FilterAdmin_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyFilterC.h>

//! Frontend for the FilterAdmin interface.
class FilterAdmin : public Item
{
  Q_OBJECT;

  typedef Item Super;

public:
  FilterAdmin(CosNotifyFilter::FilterAdmin_ptr _filterAdmin,
	      QObject * _parent = NULL, const char * _name = NULL);

protected:
  CosNotifyFilter::FilterAdmin_var filterAdmin_;
};
#endif // FilterAdmin_h
