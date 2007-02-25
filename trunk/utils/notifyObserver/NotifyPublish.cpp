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
#include "NotifyPublish.h"

#include <qpopupmenu.h>
#include <qmessagebox.h>

#include <sstream>

NotifyPublish::NotifyPublish(CosNotifyComm::NotifyPublish_ptr _notifyPublish,
			    QListViewItem * _parentItem,
			     QObject * _parent, const char * _name) :
  Super(_parentItem, NULL, _parent, _name),
  notifyPublish_(CosNotifyComm::NotifyPublish::_duplicate(_notifyPublish))
{
}

void
NotifyPublish::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("offer change", this, SLOT(offerChange()));
}

void
NotifyPublish::offerChange()
{
  CosNotification::EventTypeSeq added;
  CosNotification::EventTypeSeq removed;

  try {
    notifyPublish_->offer_change(added, removed);
  }
  catch (CosNotifyComm::InvalidEventType const& e) {
    std::ostringstream sstr;
    sstr << "Invalid event type: " << std::endl 
	 << e.type.domain_name << std::endl
	 << e.type.type_name << std::flush;

    QString error = sstr.str().c_str();
    QMessageBox::warning(NULL, 
			 "NotifySubscribe Interface", 
			 error);
  }
  catch (CORBA::Exception const& e) {
    std::ostringstream sstr;
    sstr << "CORBA exception: " << e << std::flush;

    QString error = sstr.str().c_str();
    QMessageBox::warning(NULL, 
			 "NotifyPublish Interface", 
			 "Error changing offer:\n" + error);
  }
}
