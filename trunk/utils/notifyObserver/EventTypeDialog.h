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
#ifndef EventTypeDialog_h
#define EventTypeDialog_h

#include <qdialog.h>

#include <orbsvcs/CosNotificationC.h>

// forward declarations
class QLineEdit;

class EventTypeDialog : public QDialog
{
  Q_OBJECT;

  typedef QDialog Super;

public:
  EventTypeDialog(CosNotification::EventType const& _eventType,
		  QWidget * _parent = NULL, const char * _name = NULL);

  CosNotification::EventType result() const;

protected:
  QLineEdit * domainName_;
  QLineEdit * typeName_;
};

#endif // EventTypeDialog_h
