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

#include "NotifyPublish.h"

#include <qpopupmenu.h>
#include <qmessagebox.h>

#include <sstream>

NotifyPublish::NotifyPublish(CosNotifyComm::NotifyPublish_ptr _notifyPublish,
			     QObject * _parent, const char * _name) :
  Super(_parent, _name),
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
