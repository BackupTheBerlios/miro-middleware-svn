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
#ifndef NotifySubscribe_h
#define NotifySubscribe_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyCommC.h>

class NotifySubscribe : public Item
{
  Q_OBJECT;

  typedef Item Super;

public:
  NotifySubscribe(CosNotifyComm::NotifySubscribe_ptr _notifySubscribe,
		  QListViewItem * _parentItem,
		  QObject * _parent = 0, const char * _name = 0);

  void contextMenu(QPopupMenu& _menu);

protected slots:
  void subscriptionChange();

protected:
  CosNotifyComm::NotifySubscribe_var notifySubscribe_;
};

#endif // NotifySubscribe_h
