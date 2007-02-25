// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
	      QListViewItem * _parentItem,
	      QObject * _parent = NULL, const char * _name = NULL);

protected:
  CosNotifyFilter::FilterAdmin_var filterAdmin_;
};
#endif // FilterAdmin_h
