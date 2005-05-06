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

#include "NotifySubscribe.h"
#include "EventTypeDialog.h"

#include <qpopupmenu.h>
#include <qmessagebox.h>

#include <sstream>

NotifySubscribe::NotifySubscribe(CosNotifyComm::NotifySubscribe_ptr _notifySubscribe,
				 QListViewItem * _parentItem,
				 QObject * _parent, const char * _name) :
  Super(_parentItem, NULL, _parent, _name),
  notifySubscribe_(CosNotifyComm::NotifySubscribe::_duplicate(_notifySubscribe))
{
}

void
NotifySubscribe::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("subscription change", this, SLOT(subscriptionChange()));
}

void
NotifySubscribe::subscriptionChange()
{
  CosNotification::EventTypeSeq added;
  CosNotification::EventTypeSeq removed;

  CosNotification::EventType dummy;
  EventTypeDialog dialog(dummy);
  dialog.exec();

  try {
    notifySubscribe_->subscription_change(added, removed);
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
			 "NotifySubscribe Interface", 
			 "Error changing subscription:\n" + error);
  }
}
